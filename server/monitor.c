#include "monitor.h"
    
    int numClients = 1;
    long bandwidth = 10000;
    long arbitrary = 0;
    long usage = 0;
    long SLA_min = 0;
    long unused = 0;
    long SLA_version = 0;
    /************************* SETTERS **********************************/
      
    /* Set the number of connected clients. */
    void setNumClients(int n){
        
    }
    
    /* Increment the number of connected clients. */
    void incrementNumClients(void){
        numClients++;
    }
    
    /* Decrement the number of connected clients. */
    void DecrementNumClients(void){
        numClients--;
    }

    /* Set the maximum bandwidth capacity. */
    void setBandwidth(int n){
        
    }

    /* Set arbitrary maximum bandwidth of client. */
    void setArbitrary(int n){
        
    }

    /* Set bandwidth being used. */
    void setUsage(int n){
        
    }

    /* Set SLA min bandwidth guaranteed. */
    void setSLA_min(int n){
        SLA_min = bandwidth / numClients;
    }

    /* Set unused bandwidth. */
    void setUnused(int n){
        unused = bandwidth - usage;
        if(unused > arbitrary){
            unused = arbitrary;
        }
    }
    
    /**
     * Increments the SLA version.
     */
    void increment_SLA(){
    	SLA_version++;
    }

    /************************* GETTERS **********************************/
    /**
     * Returns the current SLA version.
     */
    long getSLA_version(){
    	return SLA_version;
    }

    /* Return the number of connected clients. */
    int getNumClients(void){
        return numClients;
    }

    /* Return the maximum bandwidth capacity. */
    long getBandwidth(void){
        return bandwidth;
    }

    /* Return arbitrary maximum bandwidth of client. */
    long getArbitrary(void){
        return 0;
    }

    /* Return bandwidth being used. */
    long getUsage(void){
        return 0;
    }

    /* Return SLA min bandwidth guaranteed. */
    long getSLA_min(void){
        return 2;
    }

    /* Return unused bandwidth. */
    long getUnused(void){
        return 5;
    }
    
/////////////////// New as of March 15, 2016 /////////////////////
    long getMinThru(void){
        return bandwidth / numClients;
    }
    long getMaxThru(void){
        return bandwidth / numClients;
    }
    long getMinIOPS(void){
        return bandwidth / numClients;
    }
    long getMaxIOPS(void){
        return bandwidth / numClients;
    }
////////////////////// End March 15, 2016 ////////////////////////
    
    
    
    

    /********************** Watch for client compliance. *******************/
    
    /* Alert if unused bandwidth is less than zero. */
    int alert(int unused){
        if(unused < 0){
            return 1;
        } else{
            return 0;
        }
    }
    
    
    
    
    
    
    
