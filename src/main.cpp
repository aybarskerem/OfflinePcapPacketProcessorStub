/**
 * @file main.cpp
 *
 * @brief The driver code to fire up a @ref
 * PcapPacketQueueWriter to fill PcapPacketQueue and fire up
 * some threads to consume this queue and process the pcap
 * packets.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#include "PcapPacketQueueWriter.h"
#include "PacketProcessing.h"
#include "PeriodicJobController.h"
#include <iostream>
#include <ctime> // sys/time.h
#include <thread>


int main(int argc, char const *argv[])
{  
  /*
    I can use std::async instead o std::thread as well to get
    the return values via futures etc; but no need for now
   */

  /*
    Fire up a thread to fill the Singleton PcapPacketQueue
    instance

    We have to use a lambda function here since std::thread
    wouldn't know the default parameters of a function passed to
    itself given the function pointer only. As an alternative we
    can pass the argument explicitly: std::thread pcap_writer(
    writeToPcapPacketQueue, 20)
   */
  std::thread pcap_writer( 
    []() { writeToPcapPacketQueue(); } 
    );

  /* Create some pcap processor threads.
  */
  std::thread packet_processor_1 ( processPackets );
  // std::thread packet_processor_2 ( processPackets );
  // std::thread packet_processor_3 ( processPackets );

  /* Proof of we can add a job from anywhere in the code */
  struct timeval tv = {3, 0};
  auto job_id = g_ptr_periodic_class_controller_instance
    ->addJob(tv);
  std::cout << "A job with ID " << job_id
  << " and an period of " << tv.tv_sec 
  << " has been added from main.cpp." << std::endl;


    /* Proof of we can remove a job from anywhere in the code
  */
  job_id = g_ptr_periodic_class_controller_instance
    ->removeAnArbitraryJob();
  std::cout << "Job with ID " << job_id
  << "has been removed from main.cpp." << std::endl;

  /*
    Do not run continuously, shut down the program after
    sleeping some amount.
  */
  std::this_thread::sleep_for(
    std::chrono::seconds(
      Common::kProgramStayAliveTimeSeconds
      )
    );

  /* Join all the threads we fired up.
  */
  pcap_writer.join();
  packet_processor_1.join();
  // packet_processor_2.join(); 
  // packet_processor_3.join();
  return 0;
}






  

