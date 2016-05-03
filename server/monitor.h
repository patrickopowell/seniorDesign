/* Set the number of connected clients. */
void setNumClients(int);
/* Increment the number of connected clients. */
void incrementNumClients(void);
/* Decrement the number of connected clients. */
void decrementNumClients(void);
/* Set the maximum bandwidth capacity. */
void setBandwidth(int);
/* Set arbitrary maximum bandwidth of client. */
void setArbitrary(int);
/* Set bandwidth being used. */
void setUsage(int);
/* Set SLA min bandwidth guaranteed. */
void setSLA_min(int);
/* Set unused bandwidth. */
void setUnused(int);

void increment_SLA();
long getSLA_version();


/* Return the number of connected clients. */
int getNumClients(void);
/* Return the maximum bandwidth capacity. */
long getBandwidth(void);
/* Return arbitrary maximum bandwidth of client. */
long getArbitrary(void);
/* Return bandwidth being used. */
long getUsage(void);
/* Return SLA min bandwidth guaranteed. */
long getSLA_min(void);
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
