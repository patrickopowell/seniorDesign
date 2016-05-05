//#include <stdio.h>

/* Sets the head node for a linked list. */
void setHead(Node *);
/* Increment the number of connected clients. */
void incrementNumClients(void);
/* Decrement the number of connected clients. */
void decrementNumClients(void);
/* Increment SLA version. */
void increment_SLA();
/* Return the SLA version. */
long getSLA_version();
/* Return the number of connected clients. */
int getNumClients(void);
/* Return the maximum bandwidth capacity. */
long getBandwidth(void);
/* Return arbitrary maximum bandwidth of client. */
long getArbitrary(void);
/* Return bandwidth being used. */
long getUsage(Node *);
/* Return unused bandwidth. */
long getUnused(void);

/////////////////// New as of March 15, 2016 /////////////////////
long getMinThru(void); // min throughput
long getMaxThru(void); // max throughput
long getMinIOPS(void); // min IOPS
long getMaxIOPS(void); // max IOPS
////////////////////// End March 15, 2016 ////////////////////////


/* Watch for client non-compliance. */
int alert(int);
