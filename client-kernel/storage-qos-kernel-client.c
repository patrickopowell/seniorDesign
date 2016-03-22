#include "storageqos.h"

/**
*
* Storage QoS manage function - called from kernel space
*
* @return void
*
*/

void qos_manage () {
	
	unsigned int credit_use = (monitor.rops + monitor.wops) / monitor.timeelapsed;
	
	if (credit_use >= monitor.iocredits) opsqueued++; // add to queue or return false that spinlock operation cannot be terminated
	
}


/**
*
* Storage QoS read function - called from user space
*
* @return number of bytes transferred in function call
*
*/

ssize_t qos_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
	printk(KERN_INFO "qos_read() called\n");
	// set initial return to error
	ssize_t ret = -EBADF;
	
	if (file) {
		ret = vfs_read(file, buf, count, pos);
		printk(KERN_INFO "qos_read executed\n");
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
		printk(KERN_INFO "qos_write executed\n");
	}
	
	if (ret > 0) monitor.wops++;
	
	return ret;
}

static int qos_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "qos_open executed\n");
	
	if (Device_Open) return -EBUSY;

	Device_Open++;
	try_module_get(THIS_MODULE);
	
	printk(KERN_INFO "qos_open completed\n");
	
	return -2;
}

static int qos_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "qos_release executed\n");
	
	Device_Open--;
	module_put(THIS_MODULE);
	
	return 0;
}

/*
 * Start function
 */
static int __init qos_init(void)
{
	printk(KERN_INFO "Storage QoS Initialized\n");
	
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
	printk(KERN_INFO "Storage QoS Closed\n");
	unregister_chrdev(Major, DEVICE_NAME);
}

module_init(qos_init);
module_exit(qos_exit);
