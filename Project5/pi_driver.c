/*
 * "pi" minimal kernel module - /dev version
 *
 * Tyler Mohnke <tdm43@pitt.edu>
 *
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#include <asm/uaccess.h>

#include "pi.h"

/*
 * pi_read is the function called when a process calls read() on
 * /dev/hello.  It writes "Hello, world!" to the buffer passed in the
 * read() call.
 */

static ssize_t pi_read(struct file * file, char * buf, 
			  size_t count, loff_t *ppos)
{
    if((*ppos < 0) || (count < 0))
    {
        return 0;
    }
    
    int digits = *ppos + count;
    char* pPiBuffer = MALLOC(digits);
    pi(pPiBuffer, digits);
    pPiBuffer[digits - 1] = '\0';
    if (copy_to_user(buf, &pPiBuffer[*ppos], count))
    {
        FREE(pPiBuffer);
        return -EINVAL;
    }
    FREE(pPiBuffer);
    
    *ppos += count;

	return count;
}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations pi_fops = {
	.owner		= THIS_MODULE,
	.read		= pi_read,
};

static struct miscdevice pi_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/pi.
	 */
	"pi",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&pi_fops
};

static int __init
pi_init(void)
{
	int ret;

	/*
	 * Create the "pi" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/pi device using
	 * the default rules.
	 */
	ret = misc_register(&pi_dev);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \"pi\" misc device\n");

	return ret;
}

module_init(pi_init);

static void __exit
pi_exit(void)
{
	misc_deregister(&pi_dev);
}

module_exit(pi_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tyler Mohnke <tdm43@pitt.edu>");
MODULE_DESCRIPTION("\"pi\" minimal module");
MODULE_VERSION("dev");
