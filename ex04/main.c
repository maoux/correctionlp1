// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hugo Eynard <heynard@student.42.fr>");
MODULE_DESCRIPTION("My second module, Hello Keyboard !");

static int	hello_keyboard_probe(struct usb_interface *intf,
				     const struct usb_device_id *id)
{
	pr_info("Hello keyboard probe\n");
	return 0;
}

static void	hello_keyboard_disconnect(struct usb_interface *intf)
{
	pr_info("Exit Hello Keyboard module...\n");
}

static const struct usb_device_id hello_keyboard_table[] = {
	{USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			    USB_INTERFACE_SUBCLASS_BOOT,
			    USB_INTERFACE_PROTOCOL_KEYBOARD
			   )
	},
	{}
};

MODULE_DEVICE_TABLE(usb, hello_keyboard_table);

static struct usb_driver hello_keyboard_driver = {
	.name = "hello_keyboard",
	.id_table = hello_keyboard_table,
	.probe = hello_keyboard_probe,
	.disconnect = hello_keyboard_disconnect
};

static int	__init init_hello_keyboard(void)
{
	pr_info("Hello Keyboard !\n");
	return usb_register(&hello_keyboard_driver);
}

static void	__exit exit_hello_keyboard(void)
{
	pr_info("GoodBye keyboard !\n");
	usb_deregister(&hello_keyboard_driver);
}

module_init(init_hello_keyboard);
module_exit(exit_hello_keyboard);
