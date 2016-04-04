#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
//#include <linux/sched.h>	/* Needed for putting processes to sleep and waking them up */
#include <linux/timekeeping.h>		/* Needed for tracking storage operation performance */
#include <linux/fs.h>		/* Needed for VFS functions */
#include <linux/delay.h>
//#include <linux/unistd.h>
#include "/home/popowell/2016springTeam28/client-kernel/redirfs/redirfs.h"		/* Include RedirFS */

static struct ratebucket {
	// generic ID to use when you have multiple ratebuckets
	int32_t rb_id; 
	// Rate at which tokens are generated per second
	int32_t rb_rate; 
	// Think of this as the number of requests that can be handle "right now"
	int32_t rb_tokens; 
	// Depth of bucket. i.e. maximum number of tokens that can accumulate
 	int32_t rb_token_cap; 
	// Timestamp when tokens were last updated. I recommend microsecond granularity  
	uint64_t rb_ts; 
};

//int nanosleep(const struct timespec *req, struct timespec *rem);
//int timespec_get(struct timespec *ts, int base);
//#define TIME_UTC 1

int init_module(void);
void cleanup_module(void);
static ssize_t qos_read(struct file *file, char __user *buf, size_t count, loff_t *pos);
static ssize_t qos_write(struct file *file, const char __user *buf, size_t count, loff_t *pos);
enum redirfs_rv  qos_open(struct inode *inode, struct file *file);
enum redirfs_rv  qos_release(struct inode *inode, struct file *file);
void qos_throttle (void);
void update_token (struct ratebucket *rb_ptr);
int qos_can_send (struct ratebucket *rb_ptr);
static long qos_get_uptime(void);

int device_ioctl(struct inode *inode, struct file *file,  unsigned int ioctl_num, unsigned long ioctl_param);
int device_open (struct inode *inode, struct file *file);
int device_release (struct inode *inode, struct file *file);

#define DEVICE_NAME "storage_qos_kernel_client"

struct qos_monitor monitor;

struct ratebucket rb;

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
	
	// read operations count
	uint32_t rops;
	// write operations count
	uint32_t wops;
	
	uint32_t opsqueued;
	
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

static struct file_operations qos_fops = {
	//.ioctl = device_ioctl,
	.open = device_open,
	.release = device_release
};

/*static struct redirfs_filter_info qos_fops = {
	owner: THIS_MODULE,
	//readdir: qos_readdir,
	read: qos_read,
	write: qos_write,
	open: qos_open,
	release: qos_release
};*/