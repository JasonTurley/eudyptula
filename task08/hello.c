#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jason Turley");
MODULE_DESCRIPTION("Creates files for the eudyptula debugfs directory.");

/* Directory entry to store the id, jiffies, and foo debugfs files */
struct dentry *hello_dentry;

static ssize_t id_read(struct file *fp, char __user *user_buf, size_t count,
			loff_t *ppos)
{
	// TODO implement me!
	return 0;
}

static ssize_t id_write(struct file *fp, const char __user *user_buf, 
				size_t count, loff_t *ppos)
{
	// TODO implement me!
	return 0;
}

/* Checks the value of the given dentry pointer. On success, returns 0. On
 * error, prints the corresponding error message and returns an error code.
 */
static int check_dentry(struct dentry *d)
{
	if (d == ERR_PTR(-ENODEV)) {
		printk(KERN_ERR "Kernel was not built with debugfs support.\n");
		return -ENODEV;
	} else if (d == NULL) {
		printk(KERN_ERR "debugfs_create_dir/file() failed...\n");
		return -ENOMEM;
	}

	return 0;
}

static struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read  = id_read,
	.write = id_write	
};

static int __init hello_init(void)
{
	struct dentry *id_file;

	/* Create the directory "eudyptula" in the debugfs root */
	hello_dentry = debugfs_create_dir("eudyptula", NULL);

	if (check_dentry(hello_dentry))
		return -1;

	/* Create the "id" task file */
	id_file = debugfs_create_file("id", 666, hello_dentry, NULL, &id_fops);

	if (check_dentry(id_file))
		return -1;

	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_DEBUG "Goodbye World!\n");
	
	/* Cleanup the created debugfs files */
	debugfs_remove_recursive(hello_dentry);

	/* Remove the directory */
	debugfs_remove(hello_dentry);
}

module_init(hello_init);
module_exit(hello_exit);
