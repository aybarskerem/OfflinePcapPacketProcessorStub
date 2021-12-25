/**
 * @file
 *
 * @brief This file contains the friend class for the @ref
 * PeriodicJobController.
 *
 * @note DO NOT DISTRIBUTE THIS HEADER FILE. 
 *
 * This is a private header file which should not be shared with
 * the non-developers or non-testers of this application. For
 * sharing the implementations without sharing the private
 * headers, this application can be converted into a library.
 * This way, this library and the all the headers except for the
 * ones under private can be distributed.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#ifndef PRIVATE_PERIODICJOBCONTROLLERFRIEND_H_INCLUDED
#define PRIVATE_PERIODICJOBCONTROLLERFRIEND_H_INCLUDED

#include "PeriodicJobController.h"
#include <memory>

/**
 * @brief Friend class to @ref PeriodicJobController to expose
 * some of private members to for testing purposes.
 *
 * This friend class comes handy since it allows us to access
 * the internal container of the controller so that we can do
 * some tests easily.
 *
 * This friend class also helps us not to expose any internal
 * data to users.
 *
 */
class PeriodicJobControllerFriend
{

public:
  /**
   * @brief Get the internal container of
   * @ref PeriodicJobController which contains currently active
   * PeriodicJobs.
   *
   * @param periodic_job_controller_ptr The
   * @ref PeriodicJobController instance to be accessed the
   * internal container of.
   *
   * @return const std::unordered_map
   * <
   * JOBID, std::shared_ptr<IPeriodicJob>
   * >& The internal container of PeriodicJobController which 
   */
  static const std::unordered_map 
  <
    JOBID, 
    std::shared_ptr<IPeriodicJob> 
  >& 
  get_active_jobs(
    std::shared_ptr<PeriodicJobController> 
    periodic_job_controller_ptr);
};

#endif // PRIVATE_PERIODICJOBCONTROLLERFRIEND_H_INCLUDED