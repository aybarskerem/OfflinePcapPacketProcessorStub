/**
 * @file
 *
 * @brief This file contains the Common::PcapPacketQueue class
 * which is used as a container of PcapPackets.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#ifndef COMMON_PCAPPACKETQUEUE_H_INCLUDED
#define COMMON_PCAPPACKETQUEUE_H_INCLUDED

#include "PcapPacket.h"
#include <deque>
#include <mutex>
#include <thread>
#include <iostream>

namespace Common
{
  /**
   * @brief A thread-safe queue-like FIFO data structure &
   * Singleton class to hold and provide Pcap packet data
   * (PcapPacket) via queue-like methods.
   *
   * Whenever this class's methods are requried; firstly
   * getInstance method of this class should be called to get
   * the one and only instance of this class and the methods
   * should be called on this instance.
   *
   * This class has an internal container holding PcapPacket
   * class instances where anyone can push or pop&read those.
   *
   */
  class PcapPacketQueue // PcapPacketQueue Singleton
  {
    private:
      std::deque <PcapPacket> m_queue;
      std::mutex m_mutex;
    
    // SINGLETON STUFF BEGIN //
    public:   
      /** 
       * @brief Get Singleton instance
       */
      static PcapPacketQueue& getInstance()
      {
          static PcapPacketQueue singleton_instance; 
          return singleton_instance;
      }
      PcapPacketQueue(PcapPacketQueue const&) = delete;
      void operator=(PcapPacketQueue const&)  = delete;
    private:
      PcapPacketQueue() {}
    // SINGLETON STUFF END // CLASS-SPECIFIC METHODS BEGIN //
    public:
      /**
       * @brief Pops (deletes) and returns the oldest PcapPacket
       * instance from the internal queue
       *
       * @note Do not discard the returned packet or else it is
       * lost.
       *
       * @return A pseudo-packet with zero initialized fields ON
       * FAILURE    
       * @return The oldest packet (the first arrived in the
       * internal queue) ON SUCCESS
       */
      /*[[gnu::warn_unused_result]]*/
      [[nodiscard]]
      PcapPacket popPacket() 
      { 
        /*superior version of lock_guard*/
        std::scoped_lock<std::mutex> lock(m_mutex);
        /* if empty, send a pcap packet with 0, 0 in time to
        indicate it is an empty packet*/
        if ( m_queue.empty() )
          return PcapPacket( {0, 0} );

        /* FIFO (serve the oldest element in the queue first) */
        const auto packet = std::move(m_queue.front());
        m_queue.pop_front();
        return packet;
      }
      /** 
       * @brief Pushes a PcapPacket instance to the internal
       * queue
       *
       * @note Move semantics are used while pushing the
       * PcapPacket, so please make sure not to use the pushed
       * packet after calling this method.
       *
       * @param PcapPacket A newly arrived PcapPacket
       */
      void pushPacket(PcapPacket&& packet)
      {
        std::scoped_lock<std::mutex> lock(m_mutex);
        /* add the newest element to the back */
        m_queue.push_back(std::move(packet) );
      }
      // CLASS-SPECIFIC METHODS END //
  };
}

#endif // COMMON_PCAPPACKETQUEUE_H_INCLUDED