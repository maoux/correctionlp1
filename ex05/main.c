// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/fs.h> //struct file_operations

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hugo Eynard <heynard@student.42.fr>");
MODULE_DESCRIPTION("My first module, Hello World !");

static struct miscdevice miscdev_42;

static ssize_t	miscdev_42_read(struct file *file, char __user *buff,
				size_t count, loff_t *ppos)
{
	int		retval;
	size_t		real_count;
	static char	*data = "heynard";

	if (count <= 0 || 7 - *ppos == 0)
		return 0;

	real_count = (count < 7 - *ppos) ? count : 7 - *ppos;
	retval = copy_to_user(buff, data + *ppos, real_count);
	if (retval)
		return (-EIO);
	*ppos += real_count;
	return real_count;
}

static ssize_t	miscdev_42_write(struct file *file, const char __user *buff,
				 size_t count, loff_t *ppos)
{
	int	retval;
	char	login[7];

	if (count != 7)
		return -EINVAL;
	retval = copy_from_user(login, buff, count);
	if (retval)
		return -EIO;
	if (strncmp(login, "heynard", 7) == 0)
		return 7;

	return -EINVAL;
}

const static struct file_operations fops = {
	.read = miscdev_42_read,
	.write = miscdev_42_write
};

static int __init fortytwo_init(void)
{
	int	retval;

	miscdev_42.minor = MISC_DYNAMIC_MINOR;
	miscdev_42.name = "fortytwo";
	miscdev_42.fops = &fops;
	retval = misc_register(&miscdev_42);
	if (retval)
		return (retval);
	pr_info("Misc driver fortytwo got minor %i\n", miscdev_42.minor);
	return 0;
}

static void __exit fortytwo_exit(void)
{
	pr_info("Misc driver fortytwo cleaning up...\n");
	misc_deregister(&miscdev_42);
}

module_init(fortytwo_init);
module_exit(fortytwo_exit);
