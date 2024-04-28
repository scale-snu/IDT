// SPDX-License-Identifier: GPL-2.0
/*
 * Transfer IDT parameter
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
#include <linux/damon.h>
#include <asm/uaccess.h>
#include <linux/sort.h>
#include <linux/idt.h>

MODULE_AUTHOR("Juneseo Chang");
MODULE_DESCRIPTION("Kernel module for IDT");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

#define BUF_SIZE				4096
#define ACTION_FILE				"idt_action"
#define STATE_FILE				"idt_state"
#define MAX_ACCESS_FILE			"idt_freq_thres"
#define NODE_FILE				"idt_node"
#define CONFIG_FILE				"idt_config"

#define IDT_REGION_STATE_SIZE 	4

/* Set in /proc/idt_node */
extern int *idt_next_demotion_node;

/* Set in /proc/idt_action */
extern long *idt_age_thres;

/* Read in /proc/idt_freq_thres */
extern long *idt_freq_thres;

/* Read in /proc/idt_state */
extern struct idt_node_stat *idt_stat;

/* Set in /proc/idt_config */
extern int idt_num_nodes;
extern int idt_demote_wmark;
extern int idt_critical_wmark;

static struct proc_dir_entry *state_file, *access_thres_file, *action_file, *node_file, *config_file;

/*
 * Callback for reading /proc/idt_action file
 */
static ssize_t proc_read_action(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	char *buf;
	int i;

	buf = kmalloc_array(BUF_SIZE, sizeof(char), GFP_KERNEL);

	if (*ppos > 0 || count < BUF_SIZE)
		return 0;

	if (!idt_num_nodes)
		idt_num_nodes = 4;

	if (!idt_age_thres) {
		idt_age_thres = kmalloc_array(idt_num_nodes, sizeof(long), GFP_KERNEL);
		for (i = 0; i < idt_num_nodes; i++)
			idt_age_thres[i] = 0;
	}

	for (i = 0; i < idt_num_nodes; i++)
		len += sprintf(buf + len, "node%d %lu\n", i, idt_age_thres[i]);

	if (copy_to_user(ubuf, buf, len))
		return -EFAULT;
	*ppos = len;

	kfree(buf);

	return len;
}

/*
 * Callback for writing /proc/idt_action file
 */
static ssize_t proc_write_action(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	int i = 0;
	long tmp;
	char *buf, *buf_sep, *sep, *ptr = NULL;

	buf = kmalloc_array(BUF_SIZE, sizeof(char), GFP_KERNEL);
	buf_sep = kmalloc_array(BUF_SIZE, sizeof(char), GFP_KERNEL);

	if (*ppos > 0 || count > BUF_SIZE)
		return -EFAULT;
	if (copy_from_user(buf, ubuf, count))
		return -EFAULT;

	if (!idt_num_nodes)
		idt_num_nodes = 4;

	if (!idt_age_thres) {
		idt_age_thres = kmalloc_array(idt_num_nodes, sizeof(long), GFP_KERNEL);
		for (i = 0; i < idt_num_nodes; i++)
			idt_age_thres[i] = 0;
	}

	strncpy(buf_sep, buf, count);
	buf_sep[count - 1] = '\0';
	sep = buf_sep;

	while ((ptr = strsep(&sep, " ")) != NULL && i < idt_num_nodes) {
		if (kstrtol(ptr, 0, &tmp))
			return -EFAULT;

		idt_age_thres[i++] = tmp;
	}

	len = strlen(buf);
	*ppos = len;

	kfree(buf);
	kfree(buf_sep);

	return len;
}

/*
 * Callback for reading /proc/idt_freq_thres file
 */
static ssize_t proc_read_max_access(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	char *buf;
	int i;

	buf = kmalloc_array(BUF_SIZE, sizeof(char), GFP_KERNEL);

	if (*ppos > 0 || count < BUF_SIZE)
		return 0;

	if (!idt_num_nodes)
		idt_num_nodes = 4;

	if (!idt_freq_thres) {
		idt_freq_thres = kmalloc_array(idt_num_nodes, sizeof(long), GFP_KERNEL);
		memset(idt_freq_thres, 0, idt_num_nodes * sizeof(long));
	}

	for (i = 0; i < idt_num_nodes; i++)
		len += sprintf(buf + len, "node%d %lu\n", i, idt_freq_thres[i]);

	if (copy_to_user(ubuf, buf, len))
		return -EFAULT;
	*ppos = len;

	kfree(buf);

	return len;
}


/*
 * Callback for reading /proc/idt_state file
 */
static ssize_t proc_read_state(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	char *buf;
	int i;

	buf = kmalloc_array(BUF_SIZE, sizeof(char), GFP_KERNEL);

	if (*ppos > 0 || count < BUF_SIZE)
		return 0;

	if (!idt_num_nodes)
		idt_num_nodes = 4;

	if (!idt_stat) {
		idt_stat = kmalloc_array(idt_num_nodes, sizeof(struct idt_node_stat), GFP_KERNEL);
		memset(idt_stat, 0, idt_num_nodes * sizeof(struct idt_node_stat));
	}

	for (i = 0; i < idt_num_nodes; i++) {
		len += sprintf(buf + len, "[node%d]\n", i);
		len += sprintf(buf + len, "nr_regions %u\n", idt_stat[i].nr_regions);
		len += sprintf(buf + len, "age_min %u\n", idt_stat[i].age_min);
		len += sprintf(buf + len, "age_q1 %u\n", idt_stat[i].age_q1);
		len += sprintf(buf + len, "age_q2 %u\n", idt_stat[i].age_q2);
		len += sprintf(buf + len, "age_q3 %u\n", idt_stat[i].age_q3);
		len += sprintf(buf + len, "age_max %u\n", idt_stat[i].age_max);
		len += sprintf(buf + len, "demoted %llu\n", idt_stat[i].demoted_pages);
		len += sprintf(buf + len, "promoted %llu\n", idt_stat[i].promoted_pages);
		len += sprintf(buf + len, "demotion_enabled %d\n", idt_stat[i].demotion_enabled);
	}

	if (copy_to_user(ubuf, buf, len))
		return -EFAULT;
	*ppos = len;

	kfree(buf);

	return len;
}

/*
 * Callback for reading /proc/idt_node file
 */
static ssize_t proc_read_node(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	char *buf;
	int i;

	buf = kmalloc_array(BUF_SIZE, sizeof(char), GFP_KERNEL);

	if (*ppos > 0 || count < BUF_SIZE)
		return 0;

	if (!idt_num_nodes)
		idt_num_nodes = 4;

	/* Initialize demotion path */
	if (!idt_next_demotion_node) {
		idt_next_demotion_node = kmalloc_array(idt_num_nodes, sizeof(int), GFP_KERNEL);
		for (i = 0; i < idt_num_nodes; i++)
			idt_next_demotion_node[i] = NUMA_NO_NODE;
	}

	for (i = 0; i < idt_num_nodes; i++)
		len += sprintf(buf + len, "node%d->next %d\n", i, idt_next_demotion_node[i]);

	if (copy_to_user(ubuf, buf, len))
		return -EFAULT;
	*ppos = len;

	kfree(buf);

	return len;
}

/*
 * Callback for writing /proc/idt_node file
 */
static ssize_t proc_write_node(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	int tmp, i = 0;
	char *buf, *buf_sep, *sep, *ptr = NULL;

	buf = kmalloc_array(BUF_SIZE, sizeof(char), GFP_KERNEL);
	buf_sep = kmalloc_array(BUF_SIZE, sizeof(char), GFP_KERNEL);

	if (*ppos > 0 || count > BUF_SIZE)
		return -EFAULT;
	if (copy_from_user(buf, ubuf, count))
		return -EFAULT;

	if (!idt_num_nodes)
		idt_num_nodes = 4;

	/* Initialize demotion path */
	if (!idt_next_demotion_node) {
		idt_next_demotion_node = kmalloc_array(idt_num_nodes, sizeof(int), GFP_KERNEL);
		for (i = 0; i < idt_num_nodes; i++)
			idt_next_demotion_node[i] = NUMA_NO_NODE;
	}

	strncpy(buf_sep, buf, count);
	buf_sep[count - 1] = '\0';
	sep = buf_sep;

	while ((ptr = strsep(&sep, " ")) != NULL && i < idt_num_nodes) {
		if (kstrtoint(ptr, 0, &tmp))
			return -EFAULT;

		idt_next_demotion_node[i++] = tmp;
	}

	len = strlen(buf);
	*ppos = len;

	kfree(buf);
	kfree(buf_sep);

	return len;
}

/*
 * Callback for reading /proc/idt_config file
 */
static ssize_t proc_read_config(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	char *buf;

	buf = kmalloc_array(BUF_SIZE, sizeof(char), GFP_KERNEL);

	if (*ppos > 0 || count < BUF_SIZE)
		return 0;

	len += sprintf(buf, "num_nodes %d\ndemote_wmark %d\npromote_wmark %d\naggresive_demote_wmark %d\n", idt_num_nodes, idt_demote_wmark, idt_critical_wmark, idt_critical_wmark);

	if (copy_to_user(ubuf, buf, len))
		return -EFAULT;
	*ppos = len;

	kfree(buf);

	return len;
}

/*
 * Callback for writing /proc/idt_node file
 */
static ssize_t proc_write_config(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos)
{
	ssize_t len = 0;
	int num;
	char *buf;

	buf = kmalloc_array(BUF_SIZE, sizeof(char), GFP_KERNEL);

	if (*ppos > 0 || count > BUF_SIZE)
		return -EFAULT;
	if (copy_from_user(buf, ubuf, count))
		return -EFAULT;

	num = sscanf(buf, "%d %d %d", &idt_num_nodes, &idt_demote_wmark, &idt_critical_wmark);

	if (num != 3)
		return -EFAULT;

	len = strlen(buf);
	*ppos = len;

	kfree(buf);

	return len;
}

static const struct proc_ops state_ops = {
	.proc_read = proc_read_state,
	.proc_lseek	= generic_file_llseek,
};

static const struct proc_ops max_access_ops = {
	.proc_read = proc_read_max_access,
	.proc_lseek	= generic_file_llseek,
};

static const struct proc_ops action_ops = {
	.proc_read = proc_read_action,
	.proc_write = proc_write_action,
	.proc_lseek	= generic_file_llseek,
};

static const struct proc_ops node_ops = {
	.proc_read = proc_read_node,
	.proc_write = proc_write_node,
	.proc_lseek	= generic_file_llseek,
};

static const struct proc_ops config_ops = {
	.proc_read = proc_read_config,
	.proc_write = proc_write_config,
	.proc_lseek	= generic_file_llseek,
};

static int __init idt_kmodule_init(void)
{
	int ret = 0;

	state_file = proc_create(STATE_FILE, 0777, NULL, &state_ops);
	access_thres_file = proc_create(MAX_ACCESS_FILE, 0777, NULL, &max_access_ops);
	action_file = proc_create(ACTION_FILE, 0777, NULL, &action_ops);
	node_file = proc_create(NODE_FILE, 0777, NULL, &node_ops);
	config_file = proc_create(CONFIG_FILE, 0777, NULL, &config_ops);

	if (state_file == NULL || access_thres_file == NULL || action_file == NULL ||
		node_file == NULL || config_file == NULL) {
		ret = -ENOMEM;
		pr_err("Create proc entry failed\n");
		goto out;
	}

	pr_info("IDT module installed\n");

out:
	return ret;
}

static void __exit idt_kmodule_exit(void)
{
	if (state_file)
		proc_remove(state_file);
	if (access_thres_file)
		proc_remove(access_thres_file);
	if (action_file)
		proc_remove(action_file);
	if (node_file)
		proc_remove(node_file);
	if (config_file)
		proc_remove(config_file);

	pr_info("IDT module detached\n");
}

module_init(idt_kmodule_init);
module_exit(idt_kmodule_exit);
