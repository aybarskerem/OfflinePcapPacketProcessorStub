/**
 * @file
 *
 * @brief This file contains the class declaration for an
 * implementor class of @ref IPeriodicJobController declared in
 * IPeriodicJobController.h
 *
 * This file contains and extern variable @ref
 * g_ptr_periodic_class_controller_instance which is expected to
 * be used as the only instance to perform
 * @ref PeriodicJobController class methods if we want all of
 * the PeriodicJobs to be manipulated under one roof. 
 *
 * However, this class is not designed to be a Singleton. So,
 * whenever there is a need of multiple controllers, say to be
 * able to manipulate different types of PeriodicJobs in
 * different ways, then the caller code can instantiate its own
 * instance.
 *
 * The reason of using @ref
 * g_ptr_periodic_class_controller_instance is to provide a
 * single instance that can be used throughout the application
 * easily.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#ifndef PERIODICJOBCONTROLLER_H_INCLUDED
#define PERIODICJOBCONTROLLER_H_INCLUDED

#include "IPeriodicJobController.h"
#include <unordered_map>
#include <random>

/**
 * @brief thread-safe class which implements
 * IPeriodicJobController.
 *
 */
class PeriodicJobController : public IPeriodicJobController
{
  friend class PeriodicJobControllerFriend;

private:
    /**
   * @brief An internal job container to store currently
   * running jobs.
   *
   * It stores the id of the job and the job itself.
   *
   * @note This is not declared in the parent since each child
   * might want to implement their own container types.
   */

  std::unordered_map<
    JOBID, 
    std::shared_ptr<IPeriodicJob> 
    > m_active_jobs;

  /**
   * @brief How many @ref PeriodicJobs to assign upon the
   * arrival of new Pcap packet.
   *
   * This normally should not be a hard-coded value; but a
   * variable computed as a result of some computations made in
   * onNewTime method determining how many PeriodicJobs we need
   * to assign. However, for now, it is hard-coded.
   *
   * @note This is not a constant by design, normally this value
   * should be computed inside the onNewTime method.
   */
  unsigned m_no_of_jobs_to_add = 1; 

  /**
   * @brief A random number generator to be used to create
   * random period values for our newly added jobs.
   *
   * @note Normally, we should not add the period (periods)
   * randomly; but maybe after making some calculations
   * depending on the job or some constant value. But, for now
   * let's do this way.
   *
   */
  std::default_random_engine m_generator;

  /**
   * @brief Create an unique ID for the new job object
   *
   * boost::uuids::uuid is used to generate the unique job ids.
   *
   * @return JOBID "" if creation of the ID was unsucessfull.
   * @return JOBID Job ID of the newly generated job/ 
   */
  JOBID createIdForNewJob();
protected:
 /**
 * @brief Currently running jobs are stored in this container.
 */
public:
  
  [[nodiscard]] /*[[gnu::warn_unused_result]]*/
  std::vector<JOBID> onNewTime() override;
  [[nodiscard]] /*[[gnu::warn_unused_result]]*/ 
  JOBID addJob( struct timeval period ) override; 
  bool removeJob( JOBID job_id ) override;
  JOBID removeAnArbitraryJob() override;
  bool changePeriod(
    JOBID job_id, 
    struct timeval period
    ) override;
};

extern PeriodicJobController* g_ptr_periodic_class_controller_instance;

#endif // PERIODICJOBCONTROLLER_H_INCLUDED