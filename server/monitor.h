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


/* Return the number of connected clients. */
int getNumClients(void);
/* Return the maximum bandwidth capacity. */
int getBandwidth(void);
/* Return arbitrary maximum bandwidth of client. */
int getArbitrary(void);
/* Return bandwidth being used. */
int getUsage(void);
/* Return SLA min bandwidth guaranteed. */
int getSLA_min(void);
/* Return unused bandwidth. */
int getUnused(void);

/////////////////// New as of March 15, 2016 /////////////////////
int getMinThru(void); // min throughput
int getMaxThru(void); // max throughput
int getMinIOPS(void); // min IOPS
int getMaxIOPS(void); // max IOPS
////////////////////// End March 15, 2016 ////////////////////////


/* Watch for client non-compliance. */
int alert(int);
