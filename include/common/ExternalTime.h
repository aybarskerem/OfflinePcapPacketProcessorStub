/**
 * @file
 *
 * @brief This file contains the Common::ExternalTime Singleton
 * class which is used as the single source of truth as time
 * information for this project. 
 *
 * Instead of relying on the internal system time, the
 * @ref ExternalTime class in this file should be used.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */


#ifndef COMMON_EXTERNALTIME_H_INCLUDED
#define COMMON_EXTERNALTIME_H_INCLUDED

#include <ctime>
#include <mutex>
#include <thread>

namespace Common
{
  /**
   * @brief A Singleton class to hold and provide single source
   * of truth time value instead of relying on internal system
   * time for this project.
   *
   * Whenever this class's methods are requried; firstly
   * getInstance method of this class should be called to get
   * the one and only instance of this class and the methods
   * should be called on this instance.
   *
   * Whenever a @ref PcapPacket is received, its arrival time
   * info is to update the time used thorughout this project
   * instead of relying on the internal system clock. This is
   * important to make sure online & offline processing of a
   * packet behaves similar.
   *
   * Whenever a @ref PcapPacket is received, if the arrival time
   * is newer than the last time, the receiving party should
   * call set_current_time function on the Singleton instance to
   * set the ground-truth time to be stored in this class. Then
   * anyone needing the time can use get_current_time function
   * on the Singleton instance.
   *
   *
  */
  class ExternalTime // ExternalTime Singleton
  {
  // member variables
  private: 
    struct timeval m_current_time;
    std::mutex m_mutex;

  // methods
  public:
    /** 
     * @brief get Singleton instance
     */
    static ExternalTime& getInstance()
    {
        static ExternalTime singleton_instance; 
        return singleton_instance;
    }
    ExternalTime(ExternalTime const&)   = delete;
    void operator=(ExternalTime const&) = delete;

    /** Sets the internal time object to be used as ground-truth
     * time.
     *
     * Called by the pcap queue consumer to set/update the time
     * on the Singleton instance so that there is one source of
     * truth for the time throughout the application.
     *
     * @param current_time newly arrived pcap packet's arrival
     * time
     */
    void set_current_time(struct timeval current_time)
    {
      std::scoped_lock<std::mutex> lock(m_mutex);
      // // not consider microseconds for now
      if (current_time.tv_sec > m_current_time.tv_sec) 
        m_current_time = current_time;
    }

    /** Gets the internal time object to be used as ground-truth
     * time.
     *
     * Called by any code to get the single source of truth for
     * the time.
     *
     * @return internal time object
     */
    struct timeval get_current_time()
    {
      std::scoped_lock<std::mutex> lock(m_mutex);
      return m_current_time;
    }

  private:
    ExternalTime(): m_current_time({0, 0}), m_mutex()  {}
  };
}


#endif // COMMON_EXTERNALTIME_H_INCLUDED