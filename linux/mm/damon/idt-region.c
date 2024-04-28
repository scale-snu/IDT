// SPDX-License-Identifier: GPL-2.0
/*
 * IDT Region merge functions
 *
 * Author: Juneseo Chang <jschang0215@snu.ac.kr>
 */

#define pr_fmt(fmt) "idt-region: " fmt

#include <linux/idt.h>

#include "../internal.h"
#include "ops-common.h"

extern unsigned int idt_num_nodes;

/* IDT's sliding window size */
static unsigned int idt_merge_window_size = 16;


/*
 * idt_fishers_test - Perform fishers test for two ratio
 *
 * Two ratio are x1/n1 and x2/n2.
 * We highly optimized fishers test equation as a lookup table.
 * 
 * @n1:		denomiator of first ratio
 * @x1:		numerator of first ratio
 * @n2:		denomiator of second ratio
 * @x2:		numerator of second ratio
 */
static bool idt_fishers_test(unsigned long n1, unsigned long x1, unsigned long n2, unsigned long x2)
{
	switch (x1) {
        case 0: return (x2 >= 0 && x2 <= 2);
        case 1: return (x2 >= 0 && x2 <= 3);
        case 2: return (x2 >= 0 && x2 <= 4);
        case 3: return (x2 >= 1 && x2 <= 5);
        case 4: return (x2 >= 2 && x2 <= 6);
        case 5: return (x2 >= 3 && x2 <= 7);
        case 6: return (x2 >= 4 && x2 <= 8);
        case 7: return (x2 >= 5 && x2 <= 9);
        case 8: return (x2 >= 6 && x2 <= 10);
        case 9: return (x2 >= 7 && x2 <= 11);
        case 10: return (x2 >= 8 && x2 <= 12);
        case 11: return (x2 >= 9 && x2 <= 13);
        case 12: return (x2 >= 10 && x2 <= 14);
        case 13: return (x2 >= 11 && x2 <= 15);
        case 14: return (x2 >= 12 && x2 <= 16);
        case 15: return (x2 >= 13 && x2 <= 16);
        case 16: return (x2 >= 14 && x2 <= 16);
        default: return 0;
    }
}

/*
 * idt_merge_region() - determine merging two adjacent region r1 and r2
 *
 * Two regions are guaranteed to have same nid and pid.
 * We use Fisher's test to merge two region.
 * 
 * @t:		damon_target of the two regions
 * @r1:		damon_region of first region
 * @r2:		damon_region of second region
 */
bool idt_merge_region(struct damon_target *t, struct damon_region *r1, 
                    struct damon_region *r2)
{
	unsigned long x1, x2;

	if (!t || !r1 || !r2)
		return false;

	if (r1->nid < 0 || r1->nid >= idt_num_nodes)
		return false;
	if (r2->nid < 0 || r2->nid >= idt_num_nodes)
		return false;

	for (int i = 0; i < IDT_HISTOGRAM_SIZE - idt_merge_window_size; i++) {
		x1 = x2 = 0;

		for (int j = i; j < i + idt_merge_window_size; j++) {
			if (r1->access_histogram[j])
				x1++;
			if (r2->access_histogram[j])
				x2++;
		}

		/* Fast skip */
		if (abs(x1 - x2) <= 2)
			continue;

		if (!idt_fishers_test(idt_merge_window_size, x1, idt_merge_window_size, x2))
			return false;
	}

	return true;
}