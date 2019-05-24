// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kallsyms.h>
#include <linux/mount.h>
#include <linux/sched.h>
#include <linux/nsproxy.h>
#include <linux/proc_fs.h>
#include <linux/fs_struct.h>
#include <linux/slab.h>
#include <linux/namei.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hugo Eynard <heynard@student.42.fr>");
MODULE_DESCRIPTION("module create proc file with a list of mount points");

static struct proc_dir_entry	*mymounts;

static int	mymounts_display(struct vfsmount *vfsm, void *data)
{
	struct		seq_file *s;
	struct path	path = {
		.dentry = vfsm->mnt_root,
		.mnt = vfsm
	};

	s = data;
	if (vfsm->mnt_sb->s_op->show_devname)
		vfsm->mnt_sb->s_op->show_devname(s, vfsm->mnt_root);
	else
		seq_printf(s, "%s", vfsm->mnt_sb->s_id);
	if (strlen(vfsm->mnt_sb->s_id) < 8)
		seq_putc(s, '\t');
	seq_putc(s, '\t');
	seq_path(s, &path, "\t\n\\");
	seq_putc(s, '\n');
	return 0;
}

static int	mymounts_list(struct seq_file *s, void *v)
{
	struct vfsmount	*(*collect_mounts)(const struct path *p);
	int		(*iterate_mounts)(int(*)(struct vfsmount *v, void *d),
					  void *data, struct vfsmount *vfsm);
	struct path	path;
	struct vfsmount	*vfsm;
	int		retval;

	retval = kern_path("/", 0, &path);
	if (retval)
		return retval;
	collect_mounts = (void *)kallsyms_lookup_name("collect_mounts");
	iterate_mounts = (void *)kallsyms_lookup_name("iterate_mounts");
	vfsm = collect_mounts(&path);
	if (IS_ERR(vfsm))
		return PTR_ERR(vfsm);
	return iterate_mounts(mymounts_display, s, vfsm);
}

static int	mymounts_open(struct inode *inode, struct file *file)
{
	return single_open(file, mymounts_list, NULL);
}

const static struct file_operations	mymounts_ops = {
	.owner = THIS_MODULE,
	.read = seq_read,
	.open = mymounts_open
};

static int	__init mymounts_init(void)
{
	mymounts = proc_create("mymounts", 0, NULL, &mymounts_ops);
	return 0;
}

static void	__exit mymounts_cleanup(void)
{
	proc_remove(mymounts);
}

module_init(mymounts_init);
module_exit(mymounts_cleanup);
