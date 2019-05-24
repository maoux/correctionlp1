// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me> (heynard)");
MODULE_DESCRIPTION("Useless module");

static ssize_t	myfd_read(struct file *fp, char __user *user,
			  size_t size, loff_t *offs);
static ssize_t	myfd_write(struct file *fp, const char __user *user,
			   size_t size, loff_t *offs);

const static struct file_operations	myfd_ops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write
};

static struct miscdevice	myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_ops
};

char	str[PAGE_SIZE];
char	*tmp;

static int __init myfd_init(void)
{
	return misc_register(&myfd_device);
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
}

ssize_t	myfd_read(struct file *fp, char __user *user,
		  size_t size, loff_t *offs)
{
	int	j;
	size_t	i;
	char	*tmp2;

	tmp2 = kmalloc(sizeof(char) * PAGE_SIZE * 2, GFP_KERNEL);
	if (!tmp2)
		return -1;
	tmp = tmp2;
	for (j = strlen(str) - 1, i = 0; j >= 0; j--, i++)
		tmp[i] = str[j];
	tmp[i] = 0x0;
	return simple_read_from_buffer(user, size, offs, tmp, i);
}

ssize_t	myfd_write(struct file *fp, const char __user *user,
		   size_t size, loff_t *offs)
{
	ssize_t	res;

	res = simple_write_to_buffer(str, size, offs, user, size) + 1;
	str[size + 1] = 0x0;
	return res;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
