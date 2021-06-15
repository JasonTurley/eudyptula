#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jason Turley");
MODULE_DESCRIPTION("Simple hello world program.");

static int __init hello_init(void)
{
	printk(KERN_DEBUG "Hello World!");
	return 0;  /* non-zero return indicates an error */
}

static void __exit hello_exit(void)
{
	printk(KERN_DEBUG "Goodbye World!");
}

module_init(hello_init);
module_exit(hello_exit);