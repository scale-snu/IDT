// SPDX-License-Identifier: GPL-2.0
/*
 * Transfer demotion parameter and action to core.c
 *
 * Author: Juneseo Chang <jschang0215@snu.ac.kr>
 */

#define pr_fmt(fmt) "idt_kmodule: " fmt

#include <linux/module.h>
#include <linux/tty.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>

MODULE_AUTHOR("Juneseo Chang");
MODULE_DESCRIPTION("Kernel module for IDT");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

#define BUF_SIZE			100
#define ACTION_FILE		"idt_action"
#define STATE_FILE		"idt_state"
#define STAT_FILE 		"idt_stat"

static DEFINE_MUTEX(idt_state_lock);
static DEFINE_MUTEX(idt_action_lock);
static DEFINE_MUTEX(idt_stat_lock);

/* Definition from core.c */
extern unsigned long demote_min_age_region;

extern unsigned int demote_param_nr_regions;
extern unsigned int demote_param_age_min;
extern unsigned int demote_param_age_q1;
extern unsigned int demote_param_age_q2;
extern unsigned int demote_param_age_q3;
extern unsigned int demote_param_age_max;

extern unsigned long long total_promoted_pages;
extern unsigned long long total_demoted_pages;

static struct proc_dir_entry *state_file, *action_file, *stat_file;

/*
 * Callback for reading /proc/idt_action file
 */
static ssize_t proc_read_action(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	char buf[BUF_SIZE];

	if (*ppos > 0 || count < BUF_SIZE)
		return 0;

	mutex_lock(&idt_action_lock);
	len += sprintf(buf, "min_age %lu\n", demote_min_age_region);
	mutex_unlock(&idt_action_lock);

	if (copy_to_user(ubuf, buf, len))
		return -EFAULT;
	*ppos = len;

	return len;
}

/*
 * Callback for writing /proc/idt_action file
 */
static ssize_t proc_write_action(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	int num = 0;
	unsigned long _demote_min_age_region;
	char buf[BUF_SIZE];

	if (*ppos > 0 || count > BUF_SIZE)
		return -EFAULT;
	if (copy_from_user(buf, ubuf, count))
		return -EFAULT;
	num = sscanf(buf, "%lu", &_demote_min_age_region);
	if (num != 3)
		return -EFAULT;

	mutex_lock(&idt_action_lock);
	demote_min_age_region = _demote_min_age_region;
	mutex_unlock(&idt_action_lock);

	len = strlen(buf);
	*ppos = len;

	return len;
}

/*
 * Callback for reading /proc/idt_state file
 */
static ssize_t proc_read_state(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	char buf[BUF_SIZE];
	if (*ppos > 0 || count < BUF_SIZE)
		return 0;

	mutex_lock(&idt_state_lock);
	len += sprintf(buf, "age_min %u\nage_q1 %u\nage_q2 %u\nage_q3 %u\nage_max %u\nnr_regions %u\npromoted %llu\ndemoted %llu\n", demote_param_age_min, 
								demote_param_age_q1, demote_param_age_q2, demote_param_age_q3, demote_param_age_max,
								demote_param_nr_regions,total_promoted_pages, total_demoted_pages);
	mutex_unlock(&idt_state_lock);

	if (copy_to_user(ubuf, buf, len))
		return -EFAULT;
	*ppos = len;

	return len;
}

/*
 * Callback for writing /proc/idt_state file
 */
static ssize_t proc_write_state(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	int num = 0;
	unsigned int _demote_param_age_min, _demote_param_age_q1, _demote_param_age_q2, _demote_param_age_q3, _demote_param_age_max;
	unsigned int _demote_param_nr_regions;
	unsigned long long _total_promoted_pages, _total_demoted_pages;

	char buf[BUF_SIZE];

	if (*ppos > 0 || count > BUF_SIZE)
		return -EFAULT;
	if (copy_from_user(buf, ubuf, count))
		return -EFAULT;
	
	num = sscanf(buf, "%u %u %u %u %u %u %llu %llu", &_demote_param_age_min, 
								&_demote_param_age_q1, &_demote_param_age_q2, &_demote_param_age_q3, &_demote_param_age_max,
								&_demote_param_nr_regions, &_total_promoted_pages, &_total_demoted_pages);

	if (num != 8) {
		return -EFAULT;
	}

	mutex_lock(&idt_state_lock);
	demote_param_age_min = _demote_param_age_min;
	demote_param_age_q1 = _demote_param_age_q1;
	demote_param_age_q2 = _demote_param_age_q2;
	demote_param_age_q3 = _demote_param_age_q3;
	demote_param_age_max = _demote_param_age_max;

	demote_param_nr_regions = _demote_param_nr_regions;
	total_promoted_pages = _total_promoted_pages;
	total_demoted_pages = _total_demoted_pages;
	mutex_unlock(&idt_state_lock);

	len = strlen(buf);
	*ppos = len;

	return len;
}

/*
 * Callback for reading /proc/idt_stat file
 */
static ssize_t proc_read_stat(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	char buf[BUF_SIZE];
	if (*ppos > 0 || count < BUF_SIZE)
		return 0;

	mutex_lock(&idt_stat_lock);
	len += sprintf(buf, "%llu\n%llu\n", total_promoted_pages, total_demoted_pages);
	mutex_unlock(&idt_stat_lock);

	if (copy_to_user(ubuf, buf, len))
		return -EFAULT;
	*ppos = len;

	return len;
}

static const struct proc_ops state_ops = {
	.proc_read = proc_read_state,
	.proc_write = proc_write_state,
	.proc_lseek	= generic_file_llseek,
};

static const struct proc_ops action_ops = {
	.proc_read = proc_read_action,
	.proc_write = proc_write_action,
	.proc_lseek	= generic_file_llseek,
};

static const struct proc_ops stat_ops = {
	.proc_read = proc_read_stat,
	.proc_lseek	= generic_file_llseek,
};

static int __init idt_kmodule_init(void)
{
	int ret = 0;

	state_file = proc_create(STATE_FILE, 0777, NULL, &state_ops);
	action_file = proc_create(ACTION_FILE, 0777, NULL, &action_ops);
	stat_file = proc_create(STAT_FILE, 0777, NULL, &stat_ops);
	if (state_file == NULL || action_file == NULL || stat_file == NULL) {
		ret = -ENOMEM;
		pr_err("Create proc entry failed\n");
		goto out;
	}

	demote_min_age_region = 0;

	pr_info("module installed\n");

out:
	return ret;
}

static void __exit idt_kmodule_exit(void)
{
	if (state_file)
		proc_remove(state_file);
	if (action_file)
		proc_remove(action_file);
	if (stat_file)
		proc_remove(stat_file);
	pr_info("module detached\n");
}

module_init(idt_kmodule_init);
module_exit(idt_kmodule_exit);
