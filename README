# OFFLINE PCAP PACKET PROCESSOR

A skeleton code to simulate processing a pcap packet in an
offline environment while having the same behaviour as the 
packet getting processing in an online environment.  

## PROJECT DESCRIPTION
  
This is a project where in the main function, thread(s) are
spawned to fill some queue (simulating a container of some stub
pcap file entry) and other threads to consume these queue
content. Each pcap file entry stub includes a field representing
the arrival time of this packet. This packet's arrival time
information is used to update some internal time in the project
instead of relying on the system time. This is done to simulate
an online processing in an offline environment.  
  
This project uses C++17 with g++-9.3.0 compiler and expects a
boost version of 1.71.0 for the unit tests. This version value
can be changed inside the test/CMakeLists file if other boost
versions are compatible with the code.  
  
## BUILD ENVIRONMENT

The project is tested on an Ubuntu 20.04.

_libboost-dev_ is installed using apt package manager (the 
version 1.71.0) by running the command:  
`sudo apt install libboost-all-dev`  
  
## HOW TO BUILD & RUN:

### How to build:  
  
`cd build`  
`cmake ..`   
`make`  
 
### To run the main application (in build folder):  

`./offline_pcap_packet_processor`  

### To run the tests (in build folder):  

`BOOST_TEST_LOG_LEVEL=warning test/offline_pcap_packet_processor_test`

## PROJECT STRUCTURE EXPLANATION:

There are 5 main folders in this projects which are:  
1) src : To put all of our source codes except for the tests
   (.cpp files)  

2) include : To put the include files (.h file created;
The external includes such as boost etc. stays outside our  
project structure and in their installation directory.  

NOTE: Inside the "include" directory, there is a directory  
called "private". The files under this directory are for the  
developers and the testers of this application only and should  
not be distributed with anybody else since these expose internal
members of some of our classes declared in non-private headers.

3) test: Used to put our test related files (source codes,  
   CMakeLists.txt etc). The CMakeLists file in this folder is  
   called by the main CMakeLists file where this README file  
   resides.  

4) build: This is where we should configure and build our  
   application by running the cmake as "cmake .." to separate  
   our build structure from the rest of our structure. 
  
5) html: The folder containing Doxygen html documentations where
the entry html file is html/index.html. There exists a Doxyfile
in the root folder of the project from which this html 
documentation can be created. 'doxywizard' tool can be used to 
have some GUI for the usage of this Doxyfile.

## EXPLANATION FOR FILES:  
  
The detailed explanations are made in each file using C-style
/Javadoc style, Doxygen compatible comments. But here is an
overview:  
  
NOTE: Files starting with "I" are the files containing an
interface which cannot be instantiated by their own and 
expecting to be implemented by another concrete class. These are
added so that we depend on abstractions and not on concrete 
instances. This can be useful to help us follow principles like
"Open/Closed" and "Dependency Inversion" of SOLID in general.  
  
Here are explanations for some of the files:  
  
- PacketProcessing (h/cpp)  : These contain the free functions
  to be called from the main function to accept incoming packets
  and delegate the processing/consuming to corresponding
  functions/methods in the code.
  
- ExternalTime (h/cpp)        : This is to store pcap packet
  arrival time's in a shared Singleton object where we can query
  current time  (according to the latest pcap data) anywhere at 
  any time in a thread-safe manner.  
  
- PcapPacket.h         : Used to simulate a pcap packet. it is 
  kind of a stub structure expecting to be replaced by a real 
  pcap packet which could be some Ethernet frame for example.  
  
- PcapPacketQueue       : This is a custom queue-like structure
  to be able to store the received pcap files (right now,
  simulation of creating the packets to be received is provided
  by the writeToPcapPacketQueue function of
  PcapPacketQueueWriter file). This is a FIFO structure acting
  like a queue with pop and push methods.  
  
- (I)PeriodicJob (h/cpp): These files contain a(n)
  class/interface to represent our Periodic Job where run()
  function starts the job, stop() function stops this job and
  we can change the period of this job (the time between each
  processing) via changePeriod() function.   
    
- (I)PeriodicJobController (h/cpp): These files contain a(n)
  class/interface to represent our controller for our Periodic
  Jobs. To be able to control each thread separately and
  uniquely; a method called createIdForNewJob() is used to
  create an UUID. A PeriodicJobController instance variable
  called "g_ptr_periodic_class_controller_instance" exists to
  use controller functionalities from a single point of source.
  Using this instance, With this project (skeleton code), A job
  can be added or an arbitrary job can be removed. Also, a
  specific job can be removed or the period of an existing job
  can be changed given a job id.  

- PcapPacketQueueWriter : As mentioned in PcapPacketQueue, this
  file contains a function called writeToPcapPacketQueue which
  pushes some bogus pcap packet data into the PcapPacketQueue so
  that these packets can be consumed using PacketProcessing.cpp
  functions.  

- main.cpp : It is the driver of the application/project. It
  fires up a PcapPacketQueue writer thread first which
  continuously sends data (periodically indeed) to the
  PcapPacketQueue and then fires up some threads to consume
  these pcap data and trigger PeriodicJobController to create
  some PeriodicJob(s).  
