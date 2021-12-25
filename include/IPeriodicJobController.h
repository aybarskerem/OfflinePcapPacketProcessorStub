/**
 * @file
 *
 * @brief This file contains the interface for a controller of
 * the @ref PeriodicJob.
 *
 * The controller contains PeriodicJobs in an internal
 * container and supports adding, removal of these jobs or
 * chaning an period of a currently running job.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#ifndef IPERIODICJOBCONTROLLER_H_INCLUDED
#define IPERIODICJOBCONTROLLER_H_INCLUDED

#include "PeriodicJob.h"
#include <unordered_map>
#include <mutex>
#include <vector>
#include <memory> // shared_ptr

/**
 * @brief thread-safe interface class which represents a
 * controller for
 * @ref IPeriodicJob.
 *
 * This interface contains a container holding each 
 * @ref IPeriodicJob it starts and methods to add new jobs,
 * remove added ones given a specific @ref IPeriodicJob, remove
 * an arbitary one or change the period of a specified job.
 */

class IPeriodicJobController
{
protected:

  /**
   * @brief To provide thread-safety of this class; this mutex
   * should be used in all necessary methods.
   *
   */
  std::mutex m_mutex;

public:
  virtual ~IPeriodicJobController() {}

  /**
   * @brief 
   *
   * @note Do not discard the return value or else there is no
   * way to remove the job [[nodiscard]] attribute can be used
   * in c++17 (with g++ version >= 7  & proposal P0189R1 )
   *
   * @return std::vector<JOBID>& The IDs of the PeriodicJobs
   * employed upon the arrival of a new packet. The #of
   * PeriodicJobs employed is variable and depends on how many
   * we need.
   */
  /*[[gnu::warn_unused_result]]*/
  [[nodiscard]] /*[[gnu::warn_unused_result]]*/
  virtual std::vector<JOBID> onNewTime() = 0; 

    /**
   * @brief Add a new job to our internal container.
   *
   * @note Do not discard the return value or else there is no
   * way to remove the job easily.
   *
   * @param period period of the new job to be added
   * @return JOBID "" ON FAILURE .
   * @return JOBID Job ID of the newly added job ON SUCCESS
   */
  /*[[gnu::warn_unused_result]]*/
  [[nodiscard]] /*[[gnu::warn_unused_result]]*/
  virtual JOBID addJob(
    struct timeval period ) = 0;

  /**
   * @brief remove a PeriodicJob given from the internal job
   * container given the job_id.
   *
   * @note Before a PeriodicJob is removed, it is stopped as
   * well.
   *
   * @param job_id 
   * @return true  on success.
   * @return false on fail or if nothing has been removed,
   */
  virtual bool removeJob(JOBID job_id) = 0; 

  /**
   * @brief remove an arbitrary PeriodicJob from the internal
   * job container.
   *
   * @note Before a PeriodicJob is removed, it is stopped as
   * well.
   *
   * @return JOBID Job ID of the deleted job ON SUCCESS
   * @return JOBID "" ON FAILURE.
   */
  virtual JOBID removeAnArbitraryJob() = 0;

  /**
   * @brief Change the period (period) of the job given a
   * specific job.
   *
   * @param job_id the target job to change the period of.
   * @param period new period (period) for the given job
   * @return true period successfully changed.
   * @return false period changing failed.
   */
  virtual bool changePeriod(
    JOBID job_id, 
    struct timeval period) = 0;
};

#endif // IPERIODICJOBCONTROLLER_H_INCLUDED
