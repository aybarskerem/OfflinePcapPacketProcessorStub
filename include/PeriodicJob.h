/**
 * @file
 *
 * @brief This file contains the class declaration for an
 * implementor class of IPeriodicJob declared in
 * IPeriodicJob.h
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#ifndef PERIODICJOB_H_INCLUDED
#define PERIODICJOB_H_INCLUDED

#include "IPeriodicJob.h"
#include "common/PcapPacket.h"
#include <ctime>


/**
 * @brief A thread-safe class which implements 
 * @ref IPeriodicJob.
 *
 * I could have virtually inherited (i.e. virtual public
 * IPeriodicJob) from @ref IPeriodicJob in case it is possible
 * that we'd have multiple child classes inheriting from @ref
 * PeriodicJob and then another class inheriting from these
 * children which want access to @ref IPeriodicJob member
 * variables (causing two or more instance of the same member).
 * However in our code no diamond inheritance concerns exists;
 * hence no need for virtual inheritance.
 *
 */
class PeriodicJob : public IPeriodicJob
{
  private:
    /**
     * @brief Unique ID to identify each job.
     *
     * This member does not exist in the parent class because we
     * might not want to associate our jobs with some IDs.
     * Added to this child class since this way, in std::couts,
     * we can distinguish PeriodicJobs by also printing out
     * their IDs.
     */
    JOBID m_job_id;
    
    /** 
     * @brief stub method for doing some job.
     *
     * This method does not exist in the parent class because
     * each child could have different kinds of jobs to do or
     * different kinds of methods to call periodically.
     */
    void doSomeJob();
  public:
    PeriodicJob() = delete;
    PeriodicJob(struct timeval period, JOBID job_id);
    void changePeriod(struct timeval tv_period) override;
    void run() override;
    void stop() override;
    const struct timeval& get_period() override;
  
};

#endif // PERIODICJOB_H_INCLUDED