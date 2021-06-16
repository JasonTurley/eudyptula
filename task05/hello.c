/*
 * Displays a message when a USB keyboard is plugged in and removed. 
 *
 * References:
 * - https://static.lwn.net/images/pdf/LDD3/ch13.pdf
 * - https://stackoverflow.com/questions/23307579/loading-module-on-keyboard-hotplug
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jason Turley");
MODULE_DESCRIPTION("USB keyboard hello world module.");

/* Table of devices that work with this driver */
static struct usb_device_id hello_table [] = {
	/*
	 * USB_INTERFACE_INFO(class, subclass, protocol)
	 */
	{ USB_INTERFACE_INFO(
		USB_INTERFACE_CLASS_HID, 
		USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_KEYBOARD) 
	},

	{ }	/* Terminating entry */	
};

/* Necessary macro to allow user-space tools to figure out what devices this
 * driver can control */
MODULE_DEVICE_TABLE (usb, hello_table);


/**
 * Called by the USB core when it thinks it found a struct usb_interface that
 * the driver can handle.
 *
 * For our driver, simply print a debug message.
 */
static int hello_probe(struct usb_interface *intf, 
	const struct usb_device_id *id)
{
	pr_debug("Hello USB connected... hello there!\n");
	return 0;
}

/**
 * Called by the USB core when the struct usb_interface is removed from the
 * system or when the driver is being unloaded from the USB core.
 * 
 * For our driver, simply print a debug message.
 */
static void hello_disconnect(struct usb_interface *intf)
{
	pr_debug("Hello USB disconnected... goodbye!\n");
}

/* Main USB structure */
static struct usb_driver hello_driver = {
	.name = "hello_usb",
	.id_table = hello_table,
	.probe = hello_probe,
	.disconnect = hello_disconnect,
};

static int __init hello_init(void)
{
	int result;

	pr_debug("Hello World!\n");

	/* register the driver with the USB subsystem */
	result = usb_register(&hello_driver);
	if (result)
		pr_debug("usb_register failed. Error number %d\n", result);

	return result;
}

static void __exit hello_exit(void)
{
	pr_debug("Goodbye World!\n");

	/* deregister the driver from the USB subsystem */
	usb_deregister(&hello_driver);
}


module_init(hello_init);
module_exit(hello_exit);
