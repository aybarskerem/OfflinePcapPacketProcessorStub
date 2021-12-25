/**
 * @file
 *
 * @brief This file contains the implementations of the free
 * functions declared in PacketProcessing.h.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#include "PacketProcessing.h"
#include "common/PcapPacketQueue.h"
#include "common/ExternalTime.h"
#include "common/Constants.h"
#include "PeriodicJobController.h"
#include <iostream>


void processPacket(Common::PcapPacket&& packet)
{
  std::cout << "processing the packet with the arrival time of " 
    << packet.arrival_time.tv_sec <<  std::endl;
  destructPcapPacket(std::move(packet));
}

void processPackets()
{
  unsigned number_of_packets_processed = 0;
  while( true )
  {
    auto packet = Common::PcapPacketQueue::getInstance().popPacket();
    
    /* if the queue was empty, then nothing to process */
    if (packet.arrival_time.tv_sec == 0 
        && packet.arrival_time.tv_usec == 0) 
      continue;

    if ( Common::ExternalTime::getInstance().get_current_time().tv_sec 
                                        != packet.arrival_time.tv_sec)
    {
      /* Setting system-wide time to that of external one (to
      the one obtained from the latest pcap packet */
      Common::ExternalTime::getInstance().
        set_current_time(packet.arrival_time);

      /* Calling onNewTime method of our PeriodicJobController
      so that necessary PeriodicJobs are run. We can use the
      returned job ids from this method to delete the added the
      jobs if desired. */
      auto added_job_ids = 
        g_ptr_periodic_class_controller_instance->onNewTime();
      std::cout << "newly added job ids on time "
        << packet.arrival_time.tv_sec << " are: " << std::endl; 
      for(auto id: added_job_ids)
        std::cout << id << std::endl;
    }
     
    /* Packet is destructed in this function after processing
    it, so move semantics is used. */
    processPacket(std::move(packet));
    number_of_packets_processed++;
    if(number_of_packets_processed >= Common::kMaxNumberOfPacketsToProcess)
      break;
  }
}