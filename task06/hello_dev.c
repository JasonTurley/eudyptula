#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jason Turley");

#define HELLO_ID "abc123"

static struct miscdevice hello_misc;

/**
 * Returns my assigned id when the character device is read from. 
 */
static ssize_t eudyptula_read(struct file *fp, char __user *buf, size_t count, 
	loff_t *pos)
{
	return simple_read_from_buffer(buf, count, pos, 
		HELLO_ID, strlen(HELLO_ID));
}

/**
 * Compares the data written to the kernel with my assigned id. If they match,
 * then this function returns a correct write return value. Otherwise, returns
 * the "invalid value" error value.
 */
static ssize_t eudyptula_write(struct file *fp, const char __user *buf, 
	size_t count, loff_t *pos)
{
	ssize_t nread;
	char *msg = kmalloc(count + 1, GFP_KERNEL);

	if (!msg)
		return -ENOMEM;

	nread = simple_write_to_buffer(msg, count, pos, buf, count);

	if (strncmp(msg, HELLO_ID, strlen(HELLO_ID)) != 0)
		return -EINVAL;

	kfree(msg);
	return nread;
}


static const struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.read  = eudyptula_read,
	.write = eudyptula_write,	
};


static int __init eudyptula_init(void)
{
	pr_debug("eudyptula: module loaded!\n");

	/* Let the kernel pick our minor number for us */
	hello_misc.minor = MISC_DYNAMIC_MINOR;

	/* Name our misc device /dev/eudyptula */
	hello_misc.name = "eudyptula";

	/* What functions to call when a program uses file operations */
	hello_misc.fops = &hello_fops;
	
	return misc_register(&hello_misc);
}

static void __exit eudyptula_exit(void)
{
	pr_debug("eudyptula: module unloaded!\n");
	misc_deregister(&hello_misc);
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);
