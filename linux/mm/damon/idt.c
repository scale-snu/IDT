// SPDX-License-Identifier: GPL-2.0
/*
 * IDT: Intelligent Data Placement for Multi-Tiered Main Memory with Reinforcement Learning
 *
 * Author: Juneseo Chang <jschang0215@snu.ac.kr>
 */

#define pr_fmt(fmt) "idt: " fmt

#include <linux/idt.h>

#include "../internal.h"
#include "ops-common.h"

/* Number of numa nodes supported */
unsigned int idt_num_nodes __read_mostly = 4;
EXPORT_SYMBOL(idt_num_nodes);

/*
 * Watermark for enabling demotion for each numa node.
 * Demotion is enabled when free_space <= total_space * wmark (%)
 * Unit is %. Default 10%.
 */
unsigned int idt_demote_wmark = 5;
EXPORT_SYMBOL(idt_demote_wmark);

/*
 * Crical Watermark for aggresive demotion or stop promotion.
 * (Promotion) Stop promotion when free_space <= total_space * wmark (%)
 * (Aggresive Demotion) Force increment max_access when free_space <= total_space * wmark (%)
 * Unit is %. Default 1%.
 */
unsigned int idt_critical_wmark = 1;
EXPORT_SYMBOL(idt_critical_wmark);

/*
 * Next demotion node for each node.
 * Numa node nid's next demotion node is idt_next_demotion_node[nid].
 */
int *idt_next_demotion_node __read_mostly;
EXPORT_SYMBOL(idt_next_demotion_node);

/*
 * Age threshold for enabling demotion for each node.
 * Demote when region_age >= idt_age_thres[region->nid].
 */
long *idt_age_thres;
EXPORT_SYMBOL(idt_age_thres);

/*
 * Access threshold for demotion
 * Demote when region_access <= idt_freq_thres[region->nid]
 */
long *idt_freq_thres;
EXPORT_SYMBOL(idt_freq_thres);

/*
 * Stats of idt.
 * Age statistics, demoted, promoted pages.
 */
struct idt_node_stat *idt_stat;
EXPORT_SYMBOL(idt_stat);

/* Number of region that are promoted by PRP */
unsigned long long idt_prped_regions;
EXPORT_SYMBOL(idt_prped_regions);

/* Number of region that are accessed after promoted by PRP */
unsigned long long idt_accessed_prped_regions;
EXPORT_SYMBOL(idt_accessed_prped_regions);

/* Used for sorting regions according to age */
int cmpage(const void *a, const void *b)
{
	if (*(const int *)a < *(const int *)b)
		return -1;
	else if (*(const int *)a > *(const int *)b)
		return 1;
	else
		return 0;
}

/* 
 * idt_update_freq_thres() - Update max_access for each memory nodes
 *
 * Set idt_freq_thres as the min_value 
 * of entire region access value for each memory nodes.
 * 
 * @c: 		damon_ctx
 */
static void idt_update_freq_thres(struct damon_ctx *c)
{
	struct damon_target *t, *next_t;
	struct damon_region *r, *next_r;
	int i, default_access = 100000;
	bool exist_demotion_node = false;
	long *region_max_access;

	/* Fast exit function when no demotion */
	for (i = 0; i < idt_num_nodes; i++) {
		if (idt_stat[i].demotion_enabled)
			exist_demotion_node = true;
	}
	if (!exist_demotion_node)
		return;

	/* Initialize freq_thres if not */
	if (!idt_freq_thres) {
		idt_freq_thres = kmalloc_array(idt_num_nodes, sizeof(long), GFP_KERNEL);
		memset(idt_freq_thres, 0, idt_num_nodes * sizeof(long));
	}

	/* Save maximum value of nr_accesses among region */
	region_max_access = kmalloc_array(idt_num_nodes, sizeof(long), GFP_KERNEL);
	memset(region_max_access, 0, idt_num_nodes * sizeof(long));

	idt_check_node_freespace();

	/* Step 1. Initialize with large value */
	for (i = 0; i < idt_num_nodes; i++) {
		if (idt_stat[i].demotion_enabled)
			idt_freq_thres[i] = default_access;
	}

	/* Step 2. Get min access value among region */
	damon_for_each_target_safe(t, next_t, c) {
		damon_for_each_region_safe(r, next_r, t) {
			if (idt_stat[r->nid].demotion_enabled) {
				if (r->nr_accesses < idt_freq_thres[r->nid])
					idt_freq_thres[r->nid] = r->nr_accesses;
			}
			if (idt_stat[i].aggresive_demotion_enabled) {
				if (r->nr_accesses > region_max_access[r->nid])
					region_max_access[r->nid] = r->nr_accesses;
			}
		}
	}

	/* Step 3. Adjust idt_freq_thres */
	for (i = 0; i < idt_num_nodes; i++) {
		if (idt_stat[i].aggresive_demotion_enabled)
			idt_freq_thres[r->nid] = (idt_freq_thres[r->nid] + region_max_access[r->nid]) / 2;
	}

	kfree(region_max_access);
}

/*
 * idt_get_stat() - get IDT age statistics
 *
 * Update age stats for each memory node.
 * The stats are used by RL. 
 * 
 * @c:	damon_ctx
 */
void idt_get_stat(struct damon_ctx *c)
{
	struct damon_target *t, *next_t;
	struct damon_region *r, *next_r;
	unsigned int *age_idx, **ages;
	int i;

	/* Initialize idt_stat if not */
	if (!idt_stat) {
		idt_stat = kmalloc_array(idt_num_nodes, sizeof(struct idt_node_stat), GFP_KERNEL);
		memset(idt_stat, 0, sizeof(idt_stat));
	}

	if (!c)
		return;

	ages = kmalloc_array(idt_num_nodes, sizeof(unsigned int *), GFP_KERNEL);

	for (i = 0; i < idt_num_nodes; i++)
		ages[i] = kmalloc_array(REGION_STAT_ALLOC_SIZE, sizeof(unsigned int), GFP_KERNEL);

	age_idx = kmalloc_array(idt_num_nodes, sizeof(unsigned int), GFP_KERNEL);
	memset(age_idx, 0, idt_num_nodes * sizeof(unsigned int));

	/* Get age stat */
	damon_for_each_target_safe(t, next_t, c) {
		damon_for_each_region_safe(r, next_r, t) {
			/* Set nid if not set */
			if (r->nid < 0 || r->nid >= idt_num_nodes) {
				r->nid = idt_get_region_nid(t, r);
				continue;
			}

			/* Only get stat below idt_freq_thres */
			if (r->nr_accesses <= idt_freq_thres[r->nid] && age_idx[r->nid] < REGION_STAT_ALLOC_SIZE)
				ages[r->nid][age_idx[r->nid]++] = r->age;
		}
	}

	for (i = 0; i < idt_num_nodes; i++) {
		idt_stat[i].nr_regions = age_idx[i];

		if (idt_stat[i].nr_regions <= 0) {
			kfree(ages[i]);
			continue;
		}

		/* Sort ages to get min, q1, q2, q3, max values */
		sort(ages[i], age_idx[i], sizeof(unsigned int), &cmpage, NULL);

		idt_stat[i].age_min = ages[i][0];
		idt_stat[i].age_q1 = ages[i][idt_stat[i].nr_regions / 4];
		idt_stat[i].age_q2 = ages[i][idt_stat[i].nr_regions / 2];
		idt_stat[i].age_q3 = ages[i][idt_stat[i].nr_regions * 3 / 4];
		idt_stat[i].age_max = (idt_stat[i].nr_regions > 0) ? ages[i][idt_stat[i].nr_regions - 1] : ages[i][0];

		kfree(ages[i]);
	}

	kfree(age_idx);
	kfree(ages);
}

/* Isolate page from LRU and add to the page_list */
static bool isolate_page(struct page *page, struct list_head *page_list)
{
	struct page *head = compound_head(page);

	/* Do not interfere with other mappings of this page */
	if (page_mapcount(head) != 1)
		return false;

	if (isolate_lru_page(head))
		return false;

	list_add_tail(&head->lru, page_list);
	mod_node_page_state(page_pgdat(head), NR_ISOLATED_ANON + page_is_file_lru(head), thp_nr_pages(head));

	return true;
}

/*
 * migrate_pmd_entry() - mm_walk code for adding pages to list for migration
 *
 * This function follows mm_walk_ops's .pmd_entry format.
 * walk->private is the list that holds pages that will be migrated.
 */
static int migrate_pmd_entry(pmd_t *pmd, unsigned long addr, unsigned long end, struct mm_walk *walk)
{
	struct vm_area_struct *vma = walk->vma;
	struct list_head *migrate_list = walk->private;
	spinlock_t *ptl;
	struct page *page;
	pte_t *pte, *mapped_pte;
	bool ret;

	if (!vma_migratable(vma))
		return -EFAULT;

	ptl = pmd_trans_huge_lock(pmd, vma);
	if (ptl) {
		/* Bail out if THP migration is not supported. */
		if (!thp_migration_supported())
			goto thp_out;

		/* If the THP pte is under migration, do not bother it. */
		if (unlikely(is_pmd_migration_entry(*pmd)))
			goto thp_out;

		page = damon_get_page(pmd_pfn(*pmd));
		if (!page)
			goto thp_out;

		put_page(page);
thp_out:
		spin_unlock(ptl);
		return 0;
	}

	/* regular page handling */
	if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd)))
		return -EINVAL;

	mapped_pte = pte = pte_offset_map_lock(walk->mm, pmd, addr, &ptl);
	for (; addr != end; pte++, addr += PAGE_SIZE) {
		if (pte_none(*pte) || !pte_present(*pte))
			continue;

		page = damon_get_page(pte_pfn(*pte));
		if (!page)
			continue;

		ret = isolate_page(page, migrate_list);

		put_page(page);
	}
	pte_unmap_unlock(mapped_pte, ptl);
	cond_resched();

	return 0;
}

static const struct mm_walk_ops migrate_pages_walk_ops = {
	.pmd_entry = migrate_pmd_entry,
};

/*
 * idt_migrate_region() - migrate region to target_nid
 *
 * @target: 	damon_target of r
 * @r:			region to be migrated
 * @target_nid:	target memory node that r will be migrated to
 */
unsigned long long idt_migrate_region(struct damon_target *target, 
			struct damon_region *r, int target_nid)
{
	struct mm_struct *mm;
	LIST_HEAD(idt_migrate_pages);
	LIST_HEAD(pagelist);
	int page_nid, nr_pages, ret = 0;
	unsigned int nr_succeeded = 0;
	unsigned long long idt_migrated_pages = 0;
	struct page *page, *page2;

	mm = damon_get_mm(target);
	if (!mm || !target || !r)
		return 0;

	mmap_read_lock(mm);
	walk_page_range(mm, PAGE_ALIGN(r->ar.start), PAGE_ALIGN(r->ar.end), &migrate_pages_walk_ops, &idt_migrate_pages);
	mmap_read_unlock(mm);

	mmput(mm);

	if (list_empty(&idt_migrate_pages))
		return 0;

	list_for_each_entry_safe(page, page2, &idt_migrate_pages, lru) {
		list_add(&page->lru, &pagelist);
		page_nid = page_to_nid(page);

		nr_pages = thp_nr_pages(page);

		ret = migrate_pages(&pagelist, alloc_demote_page, NULL,
												target_nid, MIGRATE_SYNC, MR_DEMOTION,
												&nr_succeeded);
		if (ret) {
			if (!list_empty(&pagelist)) {
				list_del(&page->lru);
				mod_node_page_state(page_pgdat(page), NR_ISOLATED_ANON + page_is_file_lru(page), -nr_pages);
				putback_lru_page(page);
			}
		} else {
			idt_migrated_pages += nr_succeeded;
		}

		INIT_LIST_HEAD(&pagelist);
		cond_resched();
	}

	return idt_migrated_pages;
}

/*
 * idt_demote_region() - demote region r
 *
 * Returns number of demoted pages.
 * 
 * @t:	damon_target of r
 * @r:	region to be demoted
 */
unsigned long long idt_demote_region(struct damon_target *t, struct damon_region *r)
{
	int target_nid, r_nid;
	unsigned long long demoted_pages;

	r_nid = r->nid;
	if (r_nid < 0 || r_nid >= idt_num_nodes)
		return 0;

	/* Get most upper demotion node with enough space */
	for (target_nid = idt_next_demotion_node[r->nid]; target_nid < idt_num_nodes; target_nid++) {
		if (idt_stat[target_nid].promotion_enabled)
			break;
	}

	if (target_nid < 0 || target_nid >= idt_num_nodes)
		return 0;

	demoted_pages = idt_migrate_region(t, r, target_nid);

	/* Update region info */
	if (demoted_pages) {
		idt_stat[r->nid].demoted_pages += demoted_pages;
		r->age = 0;
		r->nr_accesses = 0;
		r->prev_nid = r->nid;
		r->nid = target_nid;
		r->last_demoted_time = ktime_get_ns() / 1000;
	}

	return demoted_pages;
}

/*
 * Helper furnction of idt_demote. 
 * Checks for demote conditions.
 * 
 * 1. Check if valid memory node id.
 * 2. Check if next demotion node is valid node.
 * 3. Check if idt_stat is intitialized.
 * 4. Check if demotion is enabled in current memory node.
 * 5. Check if region access is below freq_thres.
 * 6. Check for region age is below age_thres (Skip if aggresive demotion).
 * 7. Invoke idt_demote_region.
 */
static long long __idt_demote(struct damon_target *t, struct damon_region *r)
{
	unsigned long long demoted_pages;

	/* Check if region is valid */
	if (r->nid < 0 || r->nid >= idt_num_nodes)
		r->nid = idt_get_region_nid(t, r);

	/* Check next demotion node is invalid */
	if (idt_next_demotion_node[r->nid] == NUMA_NO_NODE)
		return -1;

	/* Check if stat is intialized */
	if (unlikely(!idt_stat))
		return -1;

	/* Check if region is enabled demotion */
	idt_check_node_freespace();
	if (!(idt_stat[r->nid].demotion_enabled))
		return -1;

	/* Check region access below freq_thres */
	if (r->nr_accesses > idt_freq_thres[r->nid])
		return -1;

	/*
	 * Check region age above age_thres
	 * In aggresive demtoion, don't consider age
	 */
	if (!(idt_stat[r->nid].aggresive_demotion_enabled) && (r->age < idt_age_thres[r->nid]))
		return -1;

	demoted_pages = idt_demote_region(t, r);

	return demoted_pages;
}

/*
 * idt_demote() - demote cold region from fast memory to slow memory
 *
 * Demote pages in each region to slow mem.
 *
 * @ctx:		the given context
 */
void idt_demote(struct damon_ctx *ctx)
{
	struct damon_target *t, *next_t;
	struct damon_region *r, *next_r;
	long long *demoted_pages, _demoted_pages, total_demoted_pages = 0;
	int node;

	if (!ctx)
		return;

	/* Save demoted pages for each mem node */
	demoted_pages = kmalloc_array(idt_num_nodes, sizeof(long long), GFP_KERNEL);
	memset(demoted_pages, 0, idt_num_nodes * sizeof(long long));

	idt_get_stat(ctx);
	idt_check_node_freespace();

	idt_update_freq_thres(ctx);

	damon_for_each_target_safe(t, next_t, ctx) {
		damon_for_each_region_safe(r, next_r, t) {
			_demoted_pages = __idt_demote(t, r);
			if (_demoted_pages != -1) {
				demoted_pages[r->nid] += _demoted_pages;
				total_demoted_pages += _demoted_pages;
			}
		}
	}

	for (node = 0; node < idt_num_nodes; node++) {
		/* If need agrresive demotion but there is no demoted pages */
		if (idt_stat[node].aggresive_demotion_enabled && demoted_pages[node] == 0) {
			damon_for_each_target_safe(t, next_t, ctx) {
				damon_for_each_region_safe(r, next_r, t) {
					/* Try demote all regions */
					if (r->nid == node)
						idt_demote_region(t, r);
				}
			}
		}
	}

	kfree(demoted_pages);
	
	pr_info("[Demote] Demoted %lldK pages\n", total_demoted_pages / 1000);
}
