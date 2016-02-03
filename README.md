# Functional Overview  
The end-to-end storage QoS implementation we will be doing will be based on an out-of-band management server colocated with the storage server that provides soft SLAs to clients in order to provide a more stable storage environment by involving the clients in the QoS process and providing them queuing responsibilities that are traditionally server-side and performance damaging.  

# Organization  
The repository is organized based on project components.  
* Client Kernel Module  
  * client-kernel
* Client User Space Application  
  * client
* Server User Space Application  
  * server

# Resources
* [Interim Progress Report](https://docs.google.com/document/d/1e-yBIyU6_mPGu4V_0TCLIj2AEByg0GGijpINTlQL9S4/edit?usp=sharing)
* [GitHub Wiki](https://github.ncsu.edu/engr-csc-sdc/2016springTeam28/wiki)
* [GitHub Issues](https://github.ncsu.edu/engr-csc-sdc/2016springTeam28/issues)