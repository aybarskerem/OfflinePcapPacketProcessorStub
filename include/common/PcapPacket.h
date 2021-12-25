/**
 * @file
 *
 * @brief This file contains the @ref PcapPacket class and a
 * destructor function to destruct dynamically allocated members
 * of this struct.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#ifndef COMMON_PCAPPACKET_H_INCLUDED
#define COMMON_PCAPPACKET_H_INCLUDED

#include <ctime>
#include <cstdint>

namespace Common
{
  /**
   * @brief POD pcap packet structure stub that needs to be
   * storing pcap arrival time and packet data.
   *
   * We can represent different pcap packets with this data
   * structure since data member variable can be of any size.
   *
   * Ethernet frame, IPv4 frame etc. can be inherited from
   * PcapPacket its default constructor can be deleted; but to
   * make sure PcapPacket is POD (trivial & standard layout), I
   * chose not to do like that. Having this struct as a POD type
   * could be useful in case of a need to send PcapPacket to
   * outside without a wrapper or to be able to utilize static
   * default initialization capabilities etc.
   *
   */
  typedef struct PcapPacket
  {
      /**
       * @brief Pcap packet's arrival time that can be used to
       * set project-wide time instead of relying on the system
       * clock. 
       */
      struct timeval arrival_time;
      /**
       * @brief 
       *
       * "data" member holds the binary data like the header and
       * the payload. For example, for an Ethernet frame, data
       * member would be holding a header of 14 to 18 octets and
       * a payload of upto 1500 octets; for an IPv4 packet a
       * header of 20 to 60 octets with a payload of upto 65475
       * octets, for an IPv6 packet a header of 40 octets and a
       * payload of 65535 octets (2^32-1 octets if a Jumbogram)
       * etc.
       *
       * I chose to have "data" member as a pointer (but
       * otherwise I should have done something like data[1518]
       * for ethernet and different size for every other
       * possible packet.
       *
       * @note Since "data" member is a pointer, some memory
       * needs to be allocated before storing some data in it;
       * then deleting falls under the responsibility of whoever
       * uses the packet last.
       */
      uint8_t* data;
  } PcapPacket;

  /**
   * @brief destruct the PcapPacket's dynamically allocated data
   * members.
   *
   * This is a free function and not a method inside the 
   * @ref PcapPacket since I wanted to keep @ref PcapPacket a
   * POD type.
   *
   * @param packet The @ref PcapPacket to destruct the members
   * of
   */
  inline void destructPcapPacket(PcapPacket&& packet)
  {
    delete[] packet.data;
  }
}

#endif // COMMON_PCAPPACKET_H_INCLUDED
