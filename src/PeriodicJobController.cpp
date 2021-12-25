/**
 * @file
 *
 * @brief This file contains the implementations of the methods
 * declared in PeriodicJobController.h
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#include "PeriodicJobController.h"
#include "common/Constants.h"
#include <iostream>
#include <thread>
#include <random>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp> // to_string etc.

/**
 * @brief An instance of the @ref PeriodicJobController class
 * which is intended to be used thoroughout the code to call the
 * @ref PeriodicJobController methods on.
 *
 * @note Other @ref PeriodicJobController instances can be
 * instantiated; but it must be noted that each job controller
 * would have their own PeriodicJobs separate from each other.
 *
 */
PeriodicJobController* 
  g_ptr_periodic_class_controller_instance =
    new PeriodicJobController();

std::vector<JOBID> PeriodicJobController::onNewTime()
{
  std::vector<JOBID> newly_added_job_ids;
  /*
   * some_random_tv_sec is used to determine the period value
   * for the created PeriodicJobs.
   *
   * @note Normally this value should be computed and assigned
   * each PeriodicJob accordingly. Currently we randomly assign
   * a number within a random range ([1,4] for now) so that we
   * can have different jobs with different periods generally.
   *
   */
  __time_t some_random_tv_sec;

  std::uniform_int_distribution<int> distribution(1, 5);
  for (auto i = 0; i < m_no_of_jobs_to_add; i++)
  {
    some_random_tv_sec = distribution(m_generator);
    struct timeval some_period {some_random_tv_sec, 0};
    auto job_id = addJob( some_period );
    if(!job_id.empty())
      newly_added_job_ids.push_back(job_id);  
  }
  return newly_added_job_ids;
}

JOBID PeriodicJobController::createIdForNewJob()
{
  std::string retVal = ""; /* return value */
  constexpr unsigned number_of_max_trials_to_find_unique_id = 10;
  unsigned curr_trial_number = 0;
  while(true)
  {
    /* Implement JOBIDs using boost::uuids::uuid */
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    auto uuid_stringified = boost::uuids::to_string(uuid);
    if(m_active_jobs.find(uuid_stringified) 
                          != m_active_jobs.end() )
    {
      curr_trial_number++;
      if (curr_trial_number >= number_of_max_trials_to_find_unique_id)
        break;
    }
    else
    {
      retVal = uuid_stringified;
      break;
    }  
  }
  
  return retVal;
}

JOBID PeriodicJobController::addJob(
  struct timeval period)
{
  std::scoped_lock<std::mutex> lock(m_mutex);
  JOBID retVal = "";
  auto active_jobs_size = m_active_jobs.size();
  if (active_jobs_size >= 
      Common::kMaxNumberOfActivePeriodicJobsAllowed)
  {
      std::cout << "addJob failed, \
        total #of active jobs allowed reached!" << std::endl;
      return "";
  }
  retVal = createIdForNewJob();
  if (retVal.empty() )
  {
    std::cout << "addJob failed, \
      could not create a new job id!" << std::endl;
    return "";
  }

  auto periodic_job = std::make_shared <PeriodicJob>
                                (period, retVal);
  
  m_active_jobs.insert(
    std::make_pair(
      retVal, periodic_job)
      );
 
  std::cout << "Added a job with ID " << retVal
    << " and a period of " << period.tv_sec
    << " seconds" << std::endl;

  /* Do not wait its completion; it will be completed when the
  work is done.*/
  std::thread (&PeriodicJob::run, periodic_job).detach();  
  return retVal;
}

bool PeriodicJobController::removeJob(JOBID job_id)
{
  std::scoped_lock<std::mutex> lock(m_mutex);
  if (m_active_jobs.find(job_id) == m_active_jobs.end() )
    return false; // there is no such job
  
  m_active_jobs.at(job_id)->stop();
  m_active_jobs.erase(job_id);
  
  std::cout << "Job with ID " << job_id << 
    " has been removed" << std::endl;
  return true;
}

JOBID PeriodicJobController::removeAnArbitraryJob()
{
  std::scoped_lock<std::mutex> lock(m_mutex);
  std::string retVal = ""; /* Return Value: Deleted job ID*/

  /* If the queue is empty, there is no job to remove since
  nothing has been removed */
  if(m_active_jobs.empty())
    return "";


  /* Remove a random element from the container. Since this is
  an unordered container, removing first element is like
  removing a random element. */
  const auto itr = m_active_jobs.begin();
  retVal = itr->first;
  itr->second->stop();
  m_active_jobs.erase(retVal);
  
  std::cout << "Job with ID " << retVal 
  << " has been removed" << std::endl;

  return retVal;
}

bool PeriodicJobController::changePeriod(
  JOBID job_id, 
  struct timeval period
  )
{
  std::scoped_lock<std::mutex> lock(m_mutex);
  if(m_active_jobs.find(job_id) == m_active_jobs.end() )
    return false; // no such job_id

  m_active_jobs.at(job_id)->changePeriod(period);
  std::cout << "The period for job with ID " << job_id 
            << " has been successfully changed" << std::endl;
  
  return true;
}

// const std::unordered_map 
//   <
//     JOBID, 
//     std::shared_ptr<IPeriodicJob> 
//   >& 
//   PeriodicJobController::get_active_jobs()
//   {
//     return m_active_jobs;
//   }