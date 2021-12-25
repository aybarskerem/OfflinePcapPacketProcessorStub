/**
 * @file
 *
 * @brief This file contains the implementations of the methods
 * declared in PeriodicJobControllerFriend.h
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#include "private/PeriodicJobControllerFriend.h"

const std::unordered_map 
  <
    JOBID, 
    std::shared_ptr<IPeriodicJob> 
  >& 
  PeriodicJobControllerFriend::
    get_active_jobs(
    std::shared_ptr<PeriodicJobController> 
    periodic_job_controller_ptr)
    {
      return periodic_job_controller_ptr->m_active_jobs;
    }