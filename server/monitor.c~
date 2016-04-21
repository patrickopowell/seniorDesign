#include "monitor.h"
    
    int numClients = 0;
    int bandwidth = 0;
    int arbitrary = 0;
    int usage = 0;
    int SLA_min = 0;
    int unused = 0;
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
    
    /************************* GETTERS **********************************/
    /* Return the number of connected clients. */
    int getNumClients(void){
        return 0;
    }

    /* Return the maximum bandwidth capacity. */
    int getBandwidth(void){
        return 0;
    }

    /* Return arbitrary maximum bandwidth of client. */
    int getArbitrary(void){
        return 0;
    }

    /* Return bandwidth being used. */
    int getUsage(void){
        return 0;
    }

    /* Return SLA min bandwidth guaranteed. */
    int getSLA_min(void){
        return 2;
    }

    /* Return unused bandwidth. */
    int getUnused(void){
        return 5;
    }
    
/////////////////// New as of March 15, 2016 /////////////////////
    int getMinThru(void){
        return 20;
    }
    int getMaxThru(void){
        return 50;
    }
    int getMinIOPS(void){
        return 10;
    }
    int getMaxIOPS(void){
        return 20;
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
    
    
    
    
    
    
    
