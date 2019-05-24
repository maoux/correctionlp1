// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hugo Eynard <heynard@student.42.fr>");
MODULE_DESCRIPTION("Debugfs module");

static struct dentry	*entry_42;
static struct mutex	locker;
static char		data[PAGE_SIZE];
static size_t		data_size;

static ssize_t	read_id(struct file *file, char *buff,
			size_t count, loff_t *ppos)
{
	static char	login[7] = "heynard";
	size_t		real_count;
	int		retval;

	if (count == 0 || *ppos >= 7)
		return 0;

	real_count = (count < 7 - *ppos) ? count : 7 - *ppos;
	retval = copy_to_user(buff, login + *ppos, real_count);
	if (retval)
		return -EIO;
	*ppos += real_count;
	return real_count;
}

static ssize_t	write_id(struct file *fd, const char *str,
			 size_t count, loff_t *ppos)
{
	int	retval;
	char	login[7];

	if (count != 7)
		return -EINVAL;
	retval = copy_from_user(login, str, 7);
	if (retval)
		return -EINVAL;
	if (!strncmp(login, "heynard", 7))
		return count;
	return -EINVAL;
}

const static struct file_operations	id_ops = {
	.read = read_id,
	.write = write_id
};

static ssize_t	read_foo(struct file *fd, char *buff,
			 size_t count, loff_t *ppos)
{
	int	retval;
	int	real_count;

	if (count == 0 || *ppos >= data_size)
		return 0;

	retval = mutex_lock_interruptible(&locker);
	if (retval)
		return retval;
	real_count = *ppos + count < data_size ? count : data_size - *ppos;
	retval = copy_to_user(buff, &data[0] + *ppos, real_count);
	if (retval) {
		retval = -EIO;
	} else {
		*ppos += real_count;
		retval = real_count;
	}
	mutex_unlock(&locker);
	return retval;
}

static ssize_t	write_foo(struct file *fd, const char *str,
			  size_t count, loff_t *ppos)
{
	int	retval;
	int	real_count;

	if (count == 0 || data_size >= PAGE_SIZE)
		return 0;

	retval = mutex_lock_interruptible(&locker);
	if (retval)
		return retval;
	real_count = count + data_size < PAGE_SIZE ? count : PAGE_SIZE - count;
	retval = copy_from_user(data + data_size, str, real_count);
	if (retval) {
		data_size += retval;
		retval = -EINVAL;
	} else {
		data_size += real_count;
	}
	mutex_unlock(&locker);
	return count;
}

const static struct file_operations	foo_ops = {
	.read = read_foo,
	.write = write_foo
};

static int __init debugfs_42_init(void)
{
	mutex_init(&locker);
	entry_42 = debugfs_create_dir("fortytwo", NULL);
	if (!entry_42)
		return -ENOENT;
	if (!debugfs_create_file("id", 0664, entry_42, NULL, &id_ops))
		return -ENOENT;
	if (!debugfs_create_file("foo", 0644, entry_42, NULL, &foo_ops))
		return -ENOENT;
	if (!debugfs_create_u64("jiffies", 0444, entry_42, (u64 *)&jiffies))
		return -ENOENT;
	return 0;
}

static void __exit debugfs_42_exit(void)
{
	debugfs_remove_recursive(entry_42);
}

module_init(debugfs_42_init);
module_exit(debugfs_42_exit);
