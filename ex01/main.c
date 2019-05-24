// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hugo Eynard <heynard@student.42.fr>");
MODULE_DESCRIPTION("My first module, Hello World !");

static int __init hello_world_init(void)
{
	pr_info("Hello World !\n");
	return 0;
}

static void __exit hello_world_exit(void)
{
	pr_info("Exit Hello World module...\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);
