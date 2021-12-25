/**
 * @file
 *
 * @brief This file contains the implementations of the methods
 * declared in PeriodicJob.h.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#include "PeriodicJob.h"
#include "common/Constants.h"
#include "common/PcapPacket.h"
#include "common/ExternalTime.h"
#include "common/PcapPacketQueue.h"

#include <iostream>
#include <chrono>

PeriodicJob::PeriodicJob(
  struct timeval period, 
  JOBID job_id)
{
  m_period = period;
  m_job_id = job_id;
}

void PeriodicJob::changePeriod(struct timeval period)
{
  std::cout << "period change request for \
   the Job " << m_job_id 
   << " from an period of " << m_period.tv_sec 
   << " to "<< period.tv_sec 
   << " has been received " << std::endl;

  m_period = period;
}

void PeriodicJob::run()
{
  bool is_there_job_to_do = true;
  auto start_time   = Common::ExternalTime::getInstance().
                                            get_current_time().
                                            tv_sec;
  auto current_time = start_time; 
  while(true)
  {
    /* If stop function has been called externally, then stop
    the thread. */
    if (m_should_stop_running)
      break;

    if(is_there_job_to_do)
    {
      std::cout << "current time for the Job "
        << m_job_id << " is " << current_time 
        << " and it is time to do some job " 
        << "with a period of " << m_period.tv_sec 
        << " seconds " << std::endl; 
      
      doSomeJob();
      is_there_job_to_do = false;
    }

    /* Do not hog the CPU, sleep between each time check. */
    std::this_thread::sleep_for(
      std::chrono::milliseconds(
        Common::kPeriodicJobTimeCheckingperiod
        )
      ); 

    current_time = Common::ExternalTime::getInstance().
                                         get_current_time().
                                         tv_sec;

    /* Check if the time specified in our period variable has
    already passed. If so, execute the next cycle of this
    periodic job */
    if(current_time - start_time >= m_period.tv_sec)
    {
      is_there_job_to_do = true;
      start_time = current_time;
    }    
  }

  std::cout << "Job " << m_job_id
    << " has been stopped" << std::endl;
}

void PeriodicJob::stop()
{
  std::cout << "Job " << m_job_id 
    << " received a stop command" << std::endl;

  m_should_stop_running = true;
}

const struct timeval& PeriodicJob::get_period()
{
  return m_period;
}

void PeriodicJob::doSomeJob()
{
  /* stub code for doing something */
}

