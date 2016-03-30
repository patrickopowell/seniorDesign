#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/sched.h>	/* Needed for putting processes to sleep and waking them up */
#include <linux/time.h>		/* Needed for tracking storage operation performance */
#include <linux/fs.h>		/* Needed for VFS functions */
#include "/home/popowell/redirfs/redirfs.h"		/* Include RedirFS */

int init_module(void);
void cleanup_module(void);
static ssize_t qos_read(struct file *file, char __user *buf, size_t count, loff_t *pos);
static ssize_t qos_write(struct file *file, const char __user *buf, size_t count, loff_t *pos);
enum redirfs_rv  qos_open(struct inode *inode, struct file *file);
enum redirfs_rv  qos_release(struct inode *inode, struct file *file);
void qos_manage (void);

#define DEVICE_NAME "storage_qos_kernel_client"

struct qos_monitor monitor;

static int Major;
static int Device_Open = 0;

static redirfs_filter storageqosflt;

static struct redirfs_op_info storageqos_op_info[] = {
	{REDIRFS_REG_FOP_OPEN, qos_open, qos_open},
	{REDIRFS_REG_FOP_RELEASE, qos_release, qos_release},
	{REDIRFS_DIR_FOP_OPEN, qos_open, qos_open},
	{REDIRFS_DIR_FOP_RELEASE, qos_release, qos_release},
	//{REDIRFS_DIR_IOP_LOOKUP, qos_lookup, qos_lookup},
	{REDIRFS_OP_END, NULL, NULL}
};

struct redirfs_path_info *storageqos_path_info;

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
	
	unsigned int opsqueued;
	
	unsigned int iocredits;
	
};

static struct redirfs_filter_info storageqos_info = {
	.owner = THIS_MODULE,
	.name = "storageqosflt",
	.priority = 850000000,
	.active = 1
};

/**
*
* create new pointers for file operation functions
*
*/
/*static struct redirfs_filter_info qos_fops = {
	owner: THIS_MODULE,
	//readdir: qos_readdir,
	read: qos_read,
	write: qos_write,
	open: qos_open,
	release: qos_release
};*/