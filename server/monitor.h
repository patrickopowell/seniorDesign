/* Set the number of connected clients. */
void setNumClients(int);
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


/* Watch for client non-compliance. */
int alert(int);
