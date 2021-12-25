/**
 * @file
 *
 * @brief This file contains the interface for the PeriodicJob
 * which is a job that runs periodically with respect to the
 * time info stored in @ref ExternalTime.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#ifndef IPERIODICJOB_H_INCLUDED
#define IPERIODICJOB_H_INCLUDED

#include <ctime>
#include <string>
#include <mutex>

typedef std::string JOBID;

/**
 * @brief interface class which represents a @ref PeriodicJob
 * which can be run or stopped.
 *
 * Each job does some job periodically according to a specified
 * period between each start of the jobs. This class also
 * provides a way to change this period even after starting
 * the job.
 *
 */
class IPeriodicJob
{
protected:
  
  /**
   * @brief period (period) of the job.
   *
   * Each job does some job where there should pass "period"
   * amount of time after the start of the previous job before
   * carrying onto the next job.
   *
   */
  struct timeval m_period = {1, 0};

  /**
   * @brief if true, exit the run function 
   */
  bool m_should_stop_running = false;
public:
  /**
   * @brief Useful when destructing child object via the parent.
   *
   *
   * Define this destructor so that we can do:
   *   parent* = new
   *   child; delete parent;" 
   * where parent is the PeriodicJob class.
   *
   */
  virtual ~IPeriodicJob() { } 
  
  /**
   * @brief change the period (period) of the job 
   *
   * @param period the new period value for the job
   */
  virtual void changePeriod (struct timeval period) = 0;
  
  /**
   * @brief start the periodic job and run periodically
   */
  virtual void run() = 0;

  /**
   * @brief stop the periodic job.
   *
   * Causes "run" method to come to a halt.
   */
  virtual void stop() = 0; 

  /**
   * @brief Get the internal period object
   *
   * @return const struct timeval& 
   */
  virtual const struct timeval& get_period() = 0;
};

#endif // IPERIODICJOB_H_INCLUDED