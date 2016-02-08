#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/sched.h>	/* Needed for putting processes to sleep and waking them up */
#include <linux/time.h>		/* Needed for tracking storage operation performance */

int init_module(void);
void cleanup_module(void);
ssize_t qos_read(struct file *file, char __user *buf, size_t count, loff_t *pos);
ssize_t qos_write(struct file *file, const char __user *buf, size_t count, loff_t *pos);

/**
*
* qos_monitor will keep track of client I/O performance
*
*/
struct qos_monitor
{
	// bytes read
	ssize_t qosbytesread;
	// bytes written
	ssize_t qosbyteswritten;
	// time elapsed
	time_t timeelapsed;
}

/**
*
* create new pointers for file operation functions
*
*/
static struct file_operations File_Ops_4_Storage_Qos = {
	.read = qos_read,
	.write = qos_write,
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
	}
	
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
	}
	
	return ret;
}

/*
 * Start function
 */
int init_module(void)
{
	return 0;
}
///////// Start our project-specific code //////////




//////// End project-specific code //////////////////
/*
 * End function
 */
void cleanup_module(void)
{
	
}
