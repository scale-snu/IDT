// SPDX-License-Identifier: GPL-2.0
/*
 * IDT - Promotion mechanisms
 *
 * Author: Juneseo Chang <jschang0215@snu.ac.kr>
 */

#define pr_fmt(fmt) "idt-promote: " fmt

#include <linux/idt.h>

#include "../internal.h"
#include "ops-common.h"

extern unsigned int idt_num_nodes;
extern struct idt_node_stat *idt_stat;
extern unsigned long long idt_prped_regions;
extern unsigned long long idt_accessed_prped_regions;

/* Prevent PRP's k-NN to diverge */
static unsigned long long idt_prp_min_dist = 5;


/*
 * IDT-PRP (Predictive Region Promotion)
 *
 * Promotes region that is going to be accessed in the future.
 * Use k-Nearest Neighbor to search potential accessed region.
 *
 * @c:				damon_ctx of t
 * @t:				damon_target of target_region
 * @target_region:	target_region (goal is to promote similar region with this region)
 */
unsigned long long idt_promote_prp(struct damon_ctx *c, struct damon_target *t, struct damon_region *target_region)
{
	struct damon_region *r, *next_r, *min_dist_region = NULL;
	long long *access_dist, *vas_dist;
	struct damon_region **region_list;
	unsigned long long access_dist_avg = 0, vas_dist_avg = 0;
	unsigned long long access_dist_var = 0, vas_dist_var = 0;
	unsigned long long dist, min_dist = 0;
	unsigned int idx = 0;
	unsigned int promoted_pages = 0;

	/*
	 * Search region most similar with target_region
	 * only find region within target same target (t given as parameter).
	 * use KNN (K-Nearest Neighbor)
	 */

	access_dist = kmalloc_array(t->nr_regions + 100, sizeof(long long), GFP_KERNEL);
	vas_dist = kmalloc_array(t->nr_regions + 100, sizeof(long long), GFP_KERNEL);
	region_list = kmalloc_array(t->nr_regions + 100, sizeof(struct damon_region *), GFP_KERNEL);

	/* Step 1. collect access history and vas distance and find average */
	damon_for_each_region_safe(r, next_r, t) {
		if (unlikely(r == target_region))
			continue;

		/* Distance of virtual address (spatial locality) */
		if (r->ar.end <= target_region->ar.start)
			dist = (target_region->ar.start - r->ar.end) / 10000000;
		else
			dist = (r->ar.start - target_region->ar.end) / 10000000;

		if (dist > 100)
			continue;
		else
			vas_dist[idx] = dist;

		vas_dist_avg += vas_dist[idx];

		/* Distance between access history (temporal locality) */
		access_dist[idx] = idt_hamming_distance(r->access_histogram, target_region->access_histogram);
		access_dist_avg += access_dist[idx];

		region_list[idx] = r;

		idx++;
	}

	/* Skip if no candidate */
	if (idx == 0)
		goto out;

	access_dist_avg /= idx;
	vas_dist_avg /= idx;

	/* Avoid being 0 */
	access_dist_avg++;
	vas_dist_avg++;

	/* Step 2. find variance */
	for (int i = 0; i < idx; i++) {
		access_dist_var += (access_dist[i] - access_dist_avg) * (access_dist[i] - access_dist_avg);
		vas_dist_var += (vas_dist[i] - vas_dist_avg) * (vas_dist[i] - vas_dist_avg);
	}
	access_dist_var /= idx;
	vas_dist_var /= idx;

	/* Avoid being 0 */
	access_dist_var++;
	vas_dist_var++;

	/* Step 3. calculate normalized distance */
	for (int i = 0; i < idx; i++) {
		dist = vas_dist_var * (access_dist[i] - access_dist_avg) * (access_dist[i] - access_dist_avg) + access_dist_var * (vas_dist[i] - vas_dist_avg) * (vas_dist[i] - vas_dist_avg);

		if (unlikely(i == 0)) {
			min_dist = dist;
			min_dist_region = region_list[i];
		}
    
		if (dist < min_dist) {
			min_dist = dist;
			min_dist_region = region_list[i];
		}
	}

	/* Step 4. don't promote if searched region is in upper tier */
	if (min_dist_region->nid < 0 || min_dist_region->nid >= idt_num_nodes)
		idt_get_region_nid(t, min_dist_region);
	if (min_dist_region->nid <= target_region->nid)
		goto out;

	if (min_dist <= idt_prp_min_dist)
		idt_prp_min_dist = min_dist;
	else
		goto out;

	/* Step 4. promote searched region */
	if (min_dist_region) {
		idt_check_node_freespace();
		if (!idt_stat[0].promotion_enabled)
			goto out;

		promoted_pages = idt_promote_region(c, t, min_dist_region, false);
		if (promoted_pages) {
			idt_prped_regions++;
			min_dist_region->prped = true;
			min_dist_region->prp_dist = min_dist;
		}
	}

out:
	return promoted_pages;
}

/*
 * IDT-ARP (Access-based Region Promotion)
 *
 * Promotes region that is checked to accessed in sampling interval.
 * Directly calls idt_promote_region.
 * 
 * @c:	damon_ctx of t
 * @t:	damon_target of r
 * @r:	accessed region
 */
unsigned long long idt_promote_arp(struct damon_ctx *c, struct damon_target *t, struct damon_region *r)
{
	int target_nid = 0;
	unsigned long long arped_pages;
  
	idt_check_node_freespace();

	if (!idt_stat[target_nid].promotion_enabled)
		return 0;

	arped_pages = idt_promote_region(c, t, r, true);

	/* Invoke IDT-PRP */
	idt_promote_prp(c, t, r);

	return arped_pages;
}

/* 
 * idt_promote_region() - promote designated region to faster memory
 *
 * Invoked in idt_promote_arp or prp
 * 
 * @c: 			damon_ctx
 * @t: 			damon_target of r
 * @r:			damon_region to be promoted
 * @update_cnt:	decide whether to update idt statistics (promoted pages)
 */
unsigned long long idt_promote_region(struct damon_ctx *c, struct damon_target *t,
			struct damon_region *r, bool update_cnt)
{
	int target_nid, r_nid;
	unsigned long long promoted_pages = 0;

	r_nid = r->nid;

	target_nid = 0;

	promoted_pages = idt_migrate_region(t, r, target_nid);

	if (r->prev_nid == NUMA_NO_NODE) {
		idt_correct_region(t, r);
		goto out;
	}

	/* Update region info */
	if (promoted_pages) {
		if (update_cnt)
			idt_stat[r->prev_nid].promoted_pages += promoted_pages;

		r->age = 0;
		r->nr_accesses = 0;
		r->prev_nid = r->nid;
		r->nid = target_nid;
	}

out:
	return promoted_pages;
}

/*
 * idt_correct_region() - Promote region r that is not demoted by IDT
 *
 * Promote region if it has not been demoted but in wrong node.
 * Usually happens because of kswapd.
 * 
 * @t:	damon_target of r
 * @r: 	damon_region r that is demoted not because of IDT
 */
void idt_correct_region(struct damon_target *t, struct damon_region *r)
{
	int target_nid, r_nid;
	unsigned long long corrected_pages;

	/* Check for valid nid */
	if (r->nid < 0 || r->nid >= idt_num_nodes)
		return;

	if (!idt_stat)
		return;

	if (r->nid == IDT_DEFAULT_NODE)
		return;

	r_nid = r->nid;

	/* Get most upper node with enough space */
	idt_check_node_freespace();
	for (target_nid = 0; target_nid < r_nid; target_nid++) {
		if (idt_stat[target_nid].promotion_enabled)
			break;
	}

	if (target_nid == r_nid)
		return;

	corrected_pages = idt_migrate_region(t, r, target_nid);

	/* Update region info */
	if (corrected_pages) {
		r->age = 0;
		r->nr_accesses = 0;
		r->prev_nid = r->nid;
		r->nid = target_nid;
	}
}
