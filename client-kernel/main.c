#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/sched.h>	/* Needed for putting processes to sleep and waking them up */
#include <linux/time.h>		/* Needed for tracking storage operation performance */
#include <linux/fs.h>

int init_module(void);
void cleanup_module(void);
static ssize_t qos_read(struct file *file, char __user *buf, size_t count, loff_t *pos);
static ssize_t qos_write(struct file *file, const char __user *buf, size_t count, loff_t *pos);
static int qos_open(struct inode *inode, struct file *file);
static int qos_release(struct inode *inode, struct file *file);

struct qos_monitor monitor;

/**
*
* qos_monitor will keep track of client I/O performance
*
*/
static struct qos_monitor
{
	// bytes read
	ssize_t qosbytesread;
	// bytes written
	ssize_t qosbyteswritten;
	// time elapsed
	time_t timeelapsed;
	
	// read operations count
	int rops;
	// write operations count
	int wops;
	
};

/**
*
* create new pointers for file operation functions
*
*/
static struct file_operations qos_fops = {
	read: qos_read,
	write: qos_write,
	open: qos_open,
	release: qos_release
};

/**
*
* Storage QoS read function - called from user space
*
* @return number of bytes transferred in function call
*
*/

ssize_t qos_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
	// set initial return to error
	ssize_t ret = -EBADF;
	
	if (file) {
		ret = vfs_read(file, buf, count, pos);
		printk("qos_read executed\n");
	}
	
	if (ret > 0) monitor.rops++;
	
	return ret;
}

/**
*
* Storage QoS write function - called from user space
*
* @return number of bytes transferred in function call
*
*/

ssize_t qos_write(struct file *file, const char __user *buf, size_t count, loff_t *pos) 
{
	// set initial return to error
    ssize_t ret = -EBADF;
	
	if (file) {
		ret = vfs_write(file, buf, count, pos);
		printk("qos_write executed\n");
	}
	
	if (ret > 0) monitor.wops++;
	
	return ret;
}

static int qos_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int qos_release(struct inode *inode, struct file *file)
{
	return 0;
}

/*
 * Start function
 */
static int __init qos_init(void)
{
	printk(KERN_INFO "Storage QoS\n");
	
	Major = register_chrdev(0, DEVICE_NAME, &qos_fops);
	if (Major < 0) {
		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
		return Major;
	}
	
	return 0;
}

/*
 * End function
 */
static void __exit qos_exit(void)
{
	int ret = unregister_chrdev(Major, DEVICE_NAME);
	if (ret < 0)
		printk(KERN_ALERT "Error in unregister_chrdev: %d\n", ret);
}

module_init(qos_init);
module_exit(qos_exit);
