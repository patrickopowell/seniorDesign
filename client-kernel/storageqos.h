#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/sched.h>	/* Needed for putting processes to sleep and waking them up */
#include <linux/time.h>		/* Needed for tracking storage operation performance */
#include <linux/fs.h>		/* Needed for VFS functions */

int init_module(void);
void cleanup_module(void);
static ssize_t qos_read(struct file *file, char __user *buf, size_t count, loff_t *pos);
static ssize_t qos_write(struct file *file, const char __user *buf, size_t count, loff_t *pos);
static int qos_open(struct inode *inode, struct file *file);
static int qos_release(struct inode *inode, struct file *file);

#define DEVICE_NAME "storage_qos_kernel_client"

struct qos_monitor monitor;

static int Major;
static int Device_Open = 0;

// Create RedirFS filter for all mount points
{"/mnt", RFS_PATH_SUBTREE | RFS_PATH_INCLUDE}


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
	unsigned int timeelapsed;
	
	// read operations count
	int rops;
	// write operations count
	int wops;
	// meta operations count
	int mops;
	
	unsigned int opsqueued;
	
	unsigned int iocredits;
	
};

/**
*
* create new pointers for file operation functions
*
*/
static struct file_operations qos_fops = {
	owner: THIS_MODULE,
	//iterate: qos_iterate,
	read: qos_read,
	write: qos_write,
	open: qos_open,
	release: qos_release
};