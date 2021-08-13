/**
 * 
 * To install and create /dev/eudyptula: 
 *	make && make insert
 *
 * Test read function:
 * 	cat /dev/eudyptula
 *	abc123
 *
 * Test write function:
 * 	echo "abc123" > /dev/eudyptula && echo $?
 * 
 * To remove:
 *	make remove
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jason Turley");
MODULE_DESCRIPTION("Misc char device hello world module");

#define HELLO_ID "abc123"	/* Fictional ID number */
#define HELLO_ID_LEN 7		/* Length of ID plus NULL byte */


/**
 * Returns my assigned id when the character device is read from. 
 */
static ssize_t eudyptula_read(struct file *fp, char __user *user_buf, size_t count, 
	loff_t *pos)
{
	return simple_read_from_buffer(user_buf, count, pos, 
		HELLO_ID, strlen(HELLO_ID));
}

/**
 * Compares the data written to the kernel with my assigned id. If they match,
 * then this function returns a correct write return value. Otherwise, returns
 * the "invalid value" error value.
 */
static ssize_t eudyptula_write(struct file *fp, const char __user *user_buf, 
	size_t count, loff_t *pos)
{
	ssize_t ret;
	char buf[HELLO_ID_LEN];

	memset(buf, 0, HELLO_ID_LEN);

	/* Don't trust user data: safely copy it to `buf` */
	ret = simple_write_to_buffer(buf, HELLO_ID_LEN, pos, user_buf, count);

	/* Compare user written value to my ID */
	if (strncmp(buf, HELLO_ID, HELLO_ID_LEN -1))
		return -EINVAL;
		
	return ret;
}


static const struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.read  = eudyptula_read,
	.write = eudyptula_write
};


static struct miscdevice hello_misc = {
	MISC_DYNAMIC_MINOR,
	"eudyptula",
	&hello_fops
};

static int __init eudyptula_init(void)
{
	printk(KERN_DEBUG "hello_dev module loaded!\n");
	return misc_register(&hello_misc);
}

static void __exit eudyptula_exit(void)
{
	printk(KERN_DEBUG "hello_dev module unloaded!\n");
	misc_deregister(&hello_misc);
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);
