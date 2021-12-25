/**
 * @file
 *
 * @brief This file contains the implementations of
 * the free functions declared in PcapPacketQueueWriter.h.
 * 
 * @author Aybars Kerem TAŞKAN
 *
 */

#include "PcapPacketQueueWriter.h"
#include "common/PcapPacket.h"
#include "common/PcapPacketQueue.h"
#include "common/Constants.h"
#include <ctime>
#include <cstring> // memcpy

/**
 * @brief Push some packets to @ref PcapPacketQueue so that
 * consumers of this queue have some packets to process.
 * 
 * @param number_of_packets_to_write How many packets to push.
 */
void writeToPcapPacketQueue(unsigned number_of_packets_to_write)
{
  unsigned curr_packet_number = 0;

  /* A variable holding some random number to 
  increase pcap arrival time for each pcap packet
  by some random number */
  unsigned random_tv_offset_for_pcaps = 0;

  /* Some random number for the first PcapPacket
  arrival time */
  __time_t tv_sec = 10; 

  /* If specified #of packets are written (pushed)
    then stop writing. */
  while(curr_packet_number < number_of_packets_to_write)
  {
    struct timeval tv {tv_sec, 0};
    /*Assume a 64-octet Ethernet Frame*/
    uint8_t* data = new uint8_t[64]();
    Common::PcapPacket packet = {tv, data};
    std::cout << "pushing a pcap packet with tv_sec " 
      << packet.arrival_time.tv_sec << std::endl;
    Common::PcapPacketQueue::getInstance().
                             pushPacket( std::move(packet) );
    
    /* Do not continuously write, sleep between
    each writes */
    std::this_thread::sleep_for(
      std::chrono::milliseconds(
        Common::kPcapPacketQueueWritingperiod
      )
    );

    /* increment each pcap consective pcap packet time 
    by 0, 1 or 2 seconds (some random increment) */
    random_tv_offset_for_pcaps = curr_packet_number % 3;

    /* Increase pcap arrival time for each pcap packet
      by some random number */
    tv_sec += random_tv_offset_for_pcaps; 

    curr_packet_number++; 
  }

  std::cout << "Done with pushing packets!" << std::endl;
  
  
}