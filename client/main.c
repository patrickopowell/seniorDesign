#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */

/*
 * Start function
 */
int init_module(void)
{
	return 0;
}
///////// Start our project-specific code //////////


// Just making a change for testing purposes.

//////// End project-specific code //////////////////
/*
 * End function
 */
void cleanup_module(void)
{
	
}
