/**
 * @file
 *
 * @brief This file contains constant configuration 
 * parameters to be used throughout the application.
 * 
 * This file provides a single location to set these 
 * configuration parameters which eases our job.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#ifndef COMMON_CONSTANTS_H_INCLUDED
#define COMMON_CONSTANTS_H_INCLUDED

namespace Common
{
  /**
   * @brief At max how many packets can be consumed
   * for each call of @ref processPackets function in 
   * PacketProcessing.cpp.
   * 
   * since @ref processPackets function is designed to
   * be run in threads; this function specifies
   * how many packets should each thread calling 
   * @ref processPackets consume.
   * 
   */
  constexpr unsigned kMaxNumberOfPacketsToProcess = 6;

  /**
   * @brief The constant, defined to be pass used as the 
   * default parameter of the writeToPcapPacketQueue function in
   * PcapPacketQueueWriter.h. 
   * 
   */
  constexpr unsigned kMaxNumberOfPacketsToWrite = 20;

  /**
   * @brief How many seconds should the whole program 
   * stay alive before coming to halt.
   * 
   * This variable is used in main.cpp to sleep for this
   * many seconds after handling the code logic before 
   * shutting down the whole program.
   * 
   */
  constexpr unsigned kProgramStayAliveTimeSeconds = 20;

  /**
   * @brief Used in PeriodicJob's run function to check
   * how many milliseconds (ms) to sleep between each time
   * check.
   * 
   * Note that, ExternalTime's time updated at each second
   * at most which is 1000ms. We should sleep less than
   * 1000ms not to miss any time modification with an error
   * more than a second. Ideally, we should set this to a 
   * value which gives us best average least remainder 
   * according to module 1000 (i.e. for 400ms, we could miss 
   * the time update for 200ms after 3 sleeps, and 0ms after 5 
   * sleeps etc. even though 0ms is not reliable). So generally,
   * the less we sleep, the higher the accuracy of our periods.
   * 
   */
  constexpr unsigned kPeriodicJobTimeCheckingperiod = 300;

  /**
   * @brief How many milliseconds to wait before pushing
   * another item to the PcapPacketQueue.
   *
   * This constant is used by PcapPacketQueueWriter.h
   */
  constexpr unsigned kPcapPacketQueueWritingperiod = 300;

  /**
   * @brief #of total PeriodicJobs that can be active 
   * at the same time in a PeriodicJobController.
   * 
   * This number specifies how many active PeriodicJobs
   * can be stored in the internal queue of
   * PeriodicJobController. In case we only use
   * g_ptr_periodic_class_controller_instance declared
   * and defined in PeriodicJobController files, this
   * number specifies total number of concurrent threads
   * to run the PeriodicJobs throughout the application. 
   * 
   */
  constexpr unsigned kMaxNumberOfActivePeriodicJobsAllowed = 30;
}

#endif