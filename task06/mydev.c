#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jason Turley");
MODULE_DESCRIPTION("Simple misc char device driver");

static struct miscdevice my_dev;

/**
 *
 */
static ssize_t eudyptula_read(struct file *fp, char __user *buf, size_t size, 
	loff_t *pos)
{
	// TODO: implement me
	return 0;
}

/**
 *
 */
static ssize_t eudyptula_write(struct file *fp, const char __user *buf, 
	size_t size, loff_t *pos)
{
	// TODO: implement me
	return 0;
}


static const struct file_operations my_fops = {
	.read  = eudyptula_read,
	.write = eudyptula_write,	
};


static int __init eudyptula_init(void)
{
	int err;

	my_dev.minor = MISC_DYNAMIC_MINOR;
	my_dev.name = "eudyptula";
	my_dev.fops = &my_fops;
	
	err = misc_register(&my_dev);
	if (err)
		goto fail_printk;

	pr_debug("eudyptula: module loaded!\n");

	return 0; 

fail_printk:
	pr_err("eudyptula: misc_register failed with error number: %d\n", err);
	return err;
}

static void __exit eudyptula_exit(void)
{
	misc_deregister(&my_dev);
	pr_debug("eudyptula: module unloaded!\n");
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);
