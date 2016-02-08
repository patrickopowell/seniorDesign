#include "monitor.h"
    
    int numClients;
    int bandwidth;
    int arbitrary;
    int usage;
    int SLA_min;
    int unused;
    /************************* SETTERS **********************************/
      
    /* Set the number of connected clients. */
    void setNumClients(int n){
        
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
        
    }

    /* Return the maximum bandwidth capacity. */
    int getBandwidth(void){
        
    }

    /* Return arbitrary maximum bandwidth of client. */
    int getArbitrary(void){
        
    }

    /* Return bandwidth being used. */
    int getUsage(void){
        
    }

    /* Return SLA min bandwidth guaranteed. */
    int getSLA_min(void){
        
    }

    /* Return unused bandwidth. */
    int getUnused(void){
        
    }
    
    /********************** Watch for client compliance. *******************/
    
    /* Alert if unused bandwidth is less than zero. */
    int alert(int unused){
        if(unused < 0){
            return 1;
        } else{
            return 0;
        }
    }
    
    
    
    
    
    
    
