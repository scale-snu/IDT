// SPDX-License-Identifier: GPL-2.0
/*
 * IDT: Intelligent Data Placement for Multi-Tiered Main Memory with Reinforcement Learning
 *
 * Author: Juneseo Chang <jschang0215@snu.ac.kr>
 */

#ifndef _IDT_H_
#define _IDT_H_

#include <asm-generic/mman-common.h>
#include <linux/highmem.h>
#include <linux/hugetlb.h>
#include <linux/mmu_notifier.h>
#include <linux/page_idle.h>
#include <linux/pagewalk.h>
#include <linux/sched/mm.h>
#include <linux/migrate.h>
#include <linux/mm_inline.h>
#include <linux/sort.h>
#include <linux/swapops.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/string.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/completion.h>
#include <linux/damon.h>

#define REGION_STAT_ALLOC_SIZE (IDT_MAX_NR_REGIONS * 4)

/* Number of stats */
#define IDT_STATE_NUM 5


/*
 * Parameters used for optimizing demotion
 * @nr_regions: Total number of regions, updated at kdamond_apply_schemes
 * @age_min/q1/q2/q3/max: Stats of region age
 * @demoted_pages: Demoted pages
 * @promoted_pages: Promoted pages
 * @demotion_enabled: Current node should be demoted
 * @promotion_enabled: Can be promoted to current node
 */
struct idt_node_stat {
	unsigned int nr_regions;
    unsigned int age_min;
	unsigned int age_q1;
	unsigned int age_q2;
	unsigned int age_q3;
	unsigned int age_max;

	unsigned long long demoted_pages;
	unsigned long long promoted_pages;
	bool demotion_enabled;
	bool aggresive_demotion_enabled;
	bool promotion_enabled;
};

/* Function declaration */
void idt_get_region_info(struct damon_target *target, struct damon_region *region);
void idt_check_node_freespace(void);
void idt_get_stat(struct damon_ctx *c);
void idt_update_min_access(struct damon_ctx *c);
void idt_init(struct damon_ctx *ctx);
void idt_finish(struct damon_ctx *c);

void idt_demote(struct damon_ctx *c);
unsigned long long idt_promote_arp(struct damon_ctx *c, struct damon_target *t, struct damon_region *r);
unsigned long long idt_promote_prp(struct damon_ctx *c, struct damon_target *t, struct damon_region *r);
unsigned long long idt_promote_region(struct damon_ctx *c, struct damon_target *t, struct damon_region *r, bool update_cnt);
void idt_correct_region(struct damon_target *t, struct damon_region *r);
unsigned long long idt_migrate_region(struct damon_target *target, struct damon_region *r, int target_nid);

bool idt_merge_region(struct damon_target *t, struct damon_region *r1, struct damon_region *r2);
unsigned int idt_hamming_distance(bool *r1, bool *r2);
int idt_get_region_nid(struct damon_target *target, struct damon_region *region);

#endif /* _IDT_H */