/**
 * @file
 *
 * @brief This file contains free functions simulating pushing
 * packets to @ref PcapPacketQueue.
 *
 * In a real-world application; we do not need the functions
 * defined here. However, for this application, we need to
 * receive some pcap packets from external world. To simulate an
 * external world; these functions are used so that the @ref
 * PcapPacketQueue has packets waiting to be consumed.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#ifndef PCAPWRITER_H_INCLUDED
#define PCAPWRITER_H_INCLUDED
#include "common/Constants.h"

/**
 * @brief This function can be used to fill the PcapPacketQueue
 * so that processor threads have something to process
 *
 * @param number_of_packets_to_write 
 */
void writeToPcapPacketQueue(unsigned number_of_packets_to_write 
                            = Common::kMaxNumberOfPacketsToWrite);

#endif