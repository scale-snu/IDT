// SPDX-License-Identifier: GPL-2.0
/*
 * IDT Utility functions
 *
 * Author: Juneseo Chang <jschang0215@snu.ac.kr>
 */

#define pr_fmt(fmt) "idt-utils: " fmt

#include <linux/idt.h>
#include <linux/damon.h>
#include <linux/atomic.h>

#include "../internal.h"
#include "ops-common.h"

extern unsigned int idt_num_nodes;
extern long *idt_age_thres;
extern long *idt_freq_thres;
extern int *idt_next_demotion_node;
extern struct idt_node_stat *idt_stat;
extern unsigned int idt_demote_wmark;
extern unsigned int idt_critical_wmark;


/*
 * idt_init() - initialize IDT
 * 
 * Initialize/allocate variables.
 * Perform idt_correct_region to check if region has been demoted by kswapd
 * during IDT's initialization.
 * 
 * @ctx:	damon_ctx of current context
 */
void idt_init(struct damon_ctx *ctx)
{
	struct damon_target *t, *next_t;
	struct damon_region *r, *next_r;
	int i;

	/* Initialize num numa nodes */
	if (!idt_num_nodes)
		idt_num_nodes = 4;

	/* Initialize min_age */
	if (!idt_age_thres) {
		idt_age_thres = kmalloc_array(idt_num_nodes, sizeof(long), GFP_KERNEL);
		memset(idt_age_thres, 0, idt_num_nodes * sizeof(long));
	}

	/* Initialize max_access */
	if (!idt_freq_thres) {
		idt_freq_thres = kmalloc_array(idt_num_nodes, sizeof(long), GFP_KERNEL);
		memset(idt_freq_thres, 0, idt_num_nodes * sizeof(long));
	}

	/* Initialize demotion path */
	if (!idt_next_demotion_node) {
		idt_next_demotion_node = kmalloc_array(idt_num_nodes, sizeof(int), GFP_KERNEL);
		for (i = 0; i < idt_num_nodes; i++)
			idt_next_demotion_node[i] = NUMA_NO_NODE;
	}

	/* Initialize stat */
	if (!idt_stat) {
		idt_stat = kmalloc_array(idt_num_nodes, sizeof(struct idt_node_stat), GFP_KERNEL);
		memset(idt_stat, 0, sizeof(idt_stat));
	}

	/* Check node free space */
	idt_check_node_freespace();

	damon_for_each_target_safe(t, next_t, ctx) {
		damon_for_each_region_safe(r, next_r, t) {
			r->nid = idt_get_region_nid(t, r);
			idt_correct_region(t, r);
		}
	}

	pr_info("[IDT] Initialized\n");
}

/*
 * idt_nid_pmd_entry() - mm_walk code for getting memory node id
 *
 * This function follows mm_walk_ops's .pmd_entry format.
 * Memory node id is saved in walk->private.
 */
static int idt_nid_pmd_entry(pmd_t *pmd, unsigned long addr,
		unsigned long next, struct mm_walk *walk)
{
	pte_t *pte;
	spinlock_t *ptl;
	struct page *page;
	int *nid = walk->private;

#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	if (pmd_huge(*pmd)) {
		ptl = pmd_lock(walk->mm, pmd);
		if (!pmd_present(*pmd)) {
			spin_unlock(ptl);
			return 0;
		}

		if (!pmd_huge(*pmd)) {
			spin_unlock(ptl);
			goto regular_page;
		}
		page = damon_get_page(pmd_pfn(*pmd));
		if (!page)
			goto huge_out;
		if (pmd_young(*pmd) || !page_is_idle(page) ||
					mmu_notifier_test_young(walk->mm,
						addr)) {
			*nid = page_to_nid(page);
		}
		put_page(page);
huge_out:
		spin_unlock(ptl);
		return 0;
	}

regular_page:
#endif	/* CONFIG_TRANSPARENT_HUGEPAGE */

	if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd)))
		return -EINVAL;
	pte = pte_offset_map_lock(walk->mm, pmd, addr, &ptl);
	if (!pte_present(*pte))
		goto out;
	page = damon_get_page(pte_pfn(*pte));
	if (!page)
		goto out;
	if (pte_young(*pte) || !page_is_idle(page) ||
			mmu_notifier_test_young(walk->mm, addr)) {
		*nid = page_to_nid(page);
	}
	put_page(page);
out:
	pte_unmap_unlock(pte, ptl);
	return 0;
}

/* idt_nid_hugetlb_entry - THP support for idt_nid_pmd_entry */
#ifdef CONFIG_HUGETLB_PAGE
static int idt_nid_hugetlb_entry(pte_t *pte, unsigned long hmask,
						 unsigned long addr, unsigned long end,
						 struct mm_walk *walk)
{
	struct hstate *h = hstate_vma(walk->vma);
	struct page *page;
	spinlock_t *ptl;
	pte_t entry;
	int *nid = walk->private;

	ptl = huge_pte_lock(h, walk->mm, pte);
	entry = huge_ptep_get(pte);
	if (!pte_present(entry))
		goto out;

	page = pte_page(entry);
	if (!page)
		goto out;

	get_page(page);

	if (pte_young(entry) || !page_is_idle(page) ||
			mmu_notifier_test_young(walk->mm, addr)) {
		*nid = page_to_nid(page);
	}

	put_page(page);

out:
	spin_unlock(ptl);
	return 0;
}
#endif /* CONFIG_HUGETLB_PAGE */

static const struct mm_walk_ops idt_nid_ops = {
	.pmd_entry = idt_nid_pmd_entry,
	.hugetlb_entry = idt_nid_hugetlb_entry,
};

/*
 * idt_get_region_nid() - get memory node id of region
 *
 * @target:		damon_target of region
 * @region:		region that needs to probe memory node id
 */
int idt_get_region_nid(struct damon_target *target, struct damon_region *region)
{
	struct mm_struct *mm;
	unsigned long addr = region->ar.start;
	int page_nid = 0;

	mm = damon_get_mm(target);
	if (!mm)
		return 0;

	mmap_read_lock(mm);
	walk_page_range(mm, addr, addr + 1, &idt_nid_ops, &page_nid);
	mmap_read_unlock(mm);

	if (page_nid < 0 || page_nid >= idt_num_nodes)
		return 0;

	return page_nid;
}

/*
 * idt_check_node_freespace() - get memroy nodes' avaliable space
 * 
 * Memory nodes' info is gathered by si_meminfo_node.
 * Demotion/promotion enabled are also checked here.
 */
void idt_check_node_freespace(void)
{
	struct sysinfo *val;
	int i;
	bool res;

	if (!idt_stat) {
		idt_stat = kmalloc_array(idt_num_nodes, sizeof(struct idt_node_stat), GFP_KERNEL);
		memset(idt_stat, 0, idt_num_nodes * sizeof(struct idt_node_stat));
	}

	val = kzalloc(sizeof(struct sysinfo), GFP_KERNEL);

	/* Check node free space */
	for (i = 0; i < idt_num_nodes - 1; i++) {
		si_meminfo_node(val, i);
		res = (val->freeram <= (val->totalram * idt_demote_wmark) / 100);
		idt_stat[i].demotion_enabled = res;
		res = (val->freeram <= (val->totalram * idt_critical_wmark) / 100);
		idt_stat[i].aggresive_demotion_enabled = res;
		res = (val->freeram >= (val->totalram * idt_critical_wmark) / 100);
		idt_stat[i].promotion_enabled = res;
	}

	kfree(val);
}

/*
 * Hamming distance between regions' history vector
 * This function is used for idt_promote_prp.
 */
unsigned int idt_hamming_distance(bool *r1, bool *r2)
{
	unsigned int xor_val = 0;

	for (int i = 0; i < IDT_HISTOGRAM_SIZE; i++)
		xor_val += (r1[i] ^ r2[i]);

	return xor_val;
}

/* Free allocated variables */
void idt_finish(struct damon_ctx *c)
{
	memset(idt_stat, 0, idt_num_nodes * sizeof(struct idt_node_stat));
}
