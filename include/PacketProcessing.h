/**
 * @file
 *
 * @brief This file contains the declarations of the free
 * functions to process an incoming pcap packet.
 *
 * The functions here make sure that when a new pcap packet
 * arrives, this packet is redirected to the correct places to
 * process the packet.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#ifndef PACKETPROCESSING_H_INCLUDED
#define PACKETPROCESSING_H_INCLUDED

#include "common/PcapPacket.h"
#include <ctime>


/** 
 * @brief A free stub function which is supposed to process
 * newly arrived pcap packet and destructs the packet
 * afterwards.
 *
 * Currently, this function currently does nothing except for
 * destructing the passed packet.
 *
 * @param packet newly arrived pcap packet to process.
 */
void processPacket(Common::PcapPacket&& packet);


/** A free function which is designed to run continuously in a
 * thread and process newly arrived pcap packets.  
 *
 * This function calls uses Common::kMaxNumberOfPacketsToProcess
 * constant to determine how many packets to process. The
 * function prototype can be changed to accept parameter to
 * determine this number for each different call instead.
 */
void processPackets();


#endif // PACKETPROCESSING_H_INCLUDED