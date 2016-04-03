#include "storageqos.h"

/**
*
* Storage QoS update tokens function - called from kernel space when out of tokens
*
* @return void
*
*/

void update_tokens (struct ratebucket *rb_ptr)
{
uint64_t current_ts = now();
uint64_t time_diff;
int32_t tokens;

	time_diff = current_ts - rb_ptr->rb_ts;
        
	// This check here prevents overflow if too much time has passed
	if (time_diff > 1000000) { // More than 1 second has passed
		tokens = rb_ptr->rb_rate;
	} else {
		// time_diff is in usecs. Rate is in ops per second.
		tokens = ((int32_t)time_diff * rb_ptr->rb_rate)/1000000;
	}

	if (tokens > 0) {
		rb_ptr->rb_tokens += tokens;
		// Check credit, do not allow it to exceed cap. 
		if (rb_ptr->rb_tokens > rb_ptr->rb_token_cap) {
			rb_ptr->rb_tokens = rb_ptr->rb_token_cap;
			rb_ptr->rb_ts = current_ts;
		} else {
			// This is a tricky part. Due to rounding, we do not get the full credit we are due.
			// Advance the ratebucket timestamp based on the number of tokens generated.
			rb_ptr->rb_ts += (tokens * 1000000)/rb_ptr->rb_rate;
		}
	}
}

/**
*
* Storage QoS can send function - called from kernel space to verify token availability
*
* @return bool true if request can be sent
*
*/

int qos_can_send (struct ratebucket *rb_ptr)
{

	if (rb_ptr->rb_tokens > 0) {
		rb_ptr->rb_tokens--;
		return 1;
	}
	// Out of tokens. Update ratebucket and try again
	update_tokens(rb_ptr);
	if (rb_ptr->rb_tokens > 0) {
		rb_ptr->rb_tokens--;
		return 1;
	}

	return 0;
}


// For completeness, will check for interrupted call.
/**
*
* Storage QoS throttle function - Throttling function. Will return when caller can proceed.
*
* @return void
*
*/

//void throttle(request_t *req)
void qos_throttle (void)
{

	while(!qos_can_send(&rb)) {
		sleep_us(1000); // Some sleep function. Linux has lots to choose from.
		//if (interrupted()) { // In case user got impatient. Some Linux function that checks process state.
		//	return;
		//}
	}
	return;
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
	
	//continue when tokens available
	qos_throttle();
	
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
	
	//continue when tokens available
	qos_throttle();
	
	if (file) {
		ret = vfs_write(file, buf, count, pos);
		printk(KERN_INFO "qos_write executed\n");
	}
	
	if (ret > 0) monitor.wops++;
	
	return ret;
}

enum redirfs_rv qos_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "qos_open executed\n");
	
	if (Device_Open) return -EBUSY;
	
	//continue when tokens available
	qos_throttle();

	Device_Open++;
	try_module_get(THIS_MODULE);
	
	printk(KERN_INFO "qos_open completed\n");
	
	return -2;
}

enum redirfs_rv qos_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "qos_release executed\n");
	
	Device_Open--;
	module_put(THIS_MODULE);
	
	return 0;
}

int device_ioctl(struct inode *inode, /* see include/linux/fs.h */
 struct file *file, /* ditto */
 unsigned int ioctl_num, /* number and param for ioctl */
 unsigned long ioctl_param)
 {
	 
	 
	 
 }
 
int device_open (struct inode *inode, struct file *file)
{
	printk(KERN_INFO "device_open executed\n");
	
	if (Device_Open) return -EBUSY;

	Device_Open++;
	try_module_get(THIS_MODULE);
	
	printk(KERN_INFO "device_open completed\n");
	
	return 0;
}

int device_release (struct inode *inode, struct file *file)
{
	printk(KERN_INFO "device_release executed\n");
	
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
	
	int err;
	int rv;

	storageqosflt = redirfs_register_filter(&storageqos_info);
	if (IS_ERR(storageqosflt)) {
		rv = PTR_ERR(storageqosflt);
		printk(KERN_ERR "Storage QoS: register filter failed(%d)\n", rv);
		return rv;
	}
	
	else printk(KERN_ALERT "Filter registered!\n");

	rv = redirfs_set_operations(storageqosflt, storageqos_op_info);
	if (rv) {
		printk(KERN_ERR "Storage QoS: set operations failed(%d)\n", rv);
		//goto error;
	}
	
	else printk(KERN_ALERT "Operations set!\n");
	
	rb.rb_rate = 2000; // replace with value passed through control
	
    rb.rb_tokens = 200; // 10 percent of rate. ~100ms of iops at rate/second
	
    rb.rb_token_cap = 200; // 10 percent of rate. Controls size of bursts
	
    rb.rb_ts = now();
	
	printk(KERN_ALERT "RateBucket config set!\n");
	
	//storageqos_path_info = redirfs_get_path_info(storageqosflt, {"/home/popowell/dummyQoS", RFS_PATH_SUBTREE | RFS_PATH_INCLUDE});
	
	/*redirfs_path path;
	
	path = redirfs_get_path("/tmp");
	
	storageqos_path_info = redirfs_get_path_info(storageqosflt, path);
	
	redirfs_add_path(storageqosflt, &storageqos_path_info);
	
	redirfs_put_path(path);*/
	
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
	redirfs_unregister_filter(storageqosflt);
	redirfs_delete_filter(storageqosflt);
	printk(KERN_INFO "Storage QoS Closed\n");
	unregister_chrdev(Major, DEVICE_NAME);
}

module_init(qos_init);
module_exit(qos_exit);
