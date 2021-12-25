/**
 * @file
 *
 * @brief This file contains the BOOST tests to test the
 * methods/functions and behavior of the codes defined in src
 * and include folder.
 *
 * Boost unit_test_framework is needed to run the contents of
 * this file.
 *
 * @author Aybars Kerem TAŞKAN
 *
 */

#include "private/PeriodicJobControllerFriend.h"
#include <climits> // CHAR_BITS

// #define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE offline_pcap_packet_processor_test_name
/* use included version instead of the linked to avoid
standalone library compilation 
*/
#include <boost/test/included/unit_test.hpp> 
#include <thread>


BOOST_AUTO_TEST_SUITE( UNIT_TEST_SUITE )

/**
 * @brief Checks if the internal container size of the
 * PeriodicJobController behaves correctly.
 *
 * A set of addition and removal operations are done on the
 * controller instance for the tests. After these operations,
 * the queue size should equal to the expected values.
 *
 */
BOOST_AUTO_TEST_CASE (CONTROLLER_QUEUE_SIZE_TEST)
{
  auto ptr_periodic_class_controller_instance =
   std::make_shared<PeriodicJobController>();

  struct timeval tv = {3, 0};
  auto id1 = ptr_periodic_class_controller_instance->addJob(tv);

  
  BOOST_REQUIRE_EQUAL(
    PeriodicJobControllerFriend::get_active_jobs
  (ptr_periodic_class_controller_instance).size(),
    1
  );

    /* Check if calling addTast method was successful */
  tv = {5, 2};
  auto id2 = ptr_periodic_class_controller_instance->addJob(tv);
  auto id3 = ptr_periodic_class_controller_instance->addJob(tv);
  auto id4 = ptr_periodic_class_controller_instance->addJob(tv);

  BOOST_REQUIRE_EQUAL(
    PeriodicJobControllerFriend::get_active_jobs
  (ptr_periodic_class_controller_instance).size(),
    4
  );

  ptr_periodic_class_controller_instance->removeJob(id2);
  BOOST_REQUIRE_EQUAL(
    PeriodicJobControllerFriend::get_active_jobs
  (ptr_periodic_class_controller_instance).size(),
    3
  );

  /* after deleting a test, consequent deletions should be
  idempotent as there is no such job anymore. */
  ptr_periodic_class_controller_instance->removeJob(id2);
  ptr_periodic_class_controller_instance->removeJob(id2);
  BOOST_REQUIRE_EQUAL(
    PeriodicJobControllerFriend::get_active_jobs
  (ptr_periodic_class_controller_instance).size(),
    3
  );

  ptr_periodic_class_controller_instance->removeJob(id4);
  BOOST_REQUIRE_EQUAL(
    PeriodicJobControllerFriend::get_active_jobs
  (ptr_periodic_class_controller_instance).size(),
    2
  );

  ptr_periodic_class_controller_instance->addJob(tv);
  BOOST_REQUIRE_EQUAL(
    PeriodicJobControllerFriend::get_active_jobs
  (ptr_periodic_class_controller_instance).size(),
    3
  );

  ptr_periodic_class_controller_instance->removeAnArbitraryJob();
  BOOST_REQUIRE_EQUAL(
    PeriodicJobControllerFriend::get_active_jobs
  (ptr_periodic_class_controller_instance).size(),
    2
  );

  ptr_periodic_class_controller_instance->removeAnArbitraryJob();
  ptr_periodic_class_controller_instance->removeAnArbitraryJob();
  BOOST_REQUIRE_EQUAL(
    PeriodicJobControllerFriend::get_active_jobs
  (ptr_periodic_class_controller_instance).size(),
    0
  );

  ptr_periodic_class_controller_instance->removeAnArbitraryJob();
  ptr_periodic_class_controller_instance->removeAnArbitraryJob();
  ptr_periodic_class_controller_instance->removeAnArbitraryJob();

  /* even though we have not deleted id3 before by ID; it was
  deleted when we emptied the container by removeAnArbitraryJob
  calls, so this should have no effect */
  ptr_periodic_class_controller_instance->removeJob(id3);
  ptr_periodic_class_controller_instance->removeJob(id1);
  BOOST_REQUIRE_EQUAL(
    PeriodicJobControllerFriend::get_active_jobs
  (ptr_periodic_class_controller_instance).size(),
    0
  );

}

/**
 * @brief Test the return values of addJob, removeJob and
 * removeAnArbitraryJob for different kinds of scenarios. 
 *
 * If addition is successful, addJob should return true,
 * otherwise false & if a job is removed successfully, remove
 * methods should return true, otherwise false.
 *
 */
BOOST_AUTO_TEST_CASE (ADDITION_REMOVAL_RETURNVALUE_TEST)
{
  auto ptr_periodic_class_controller_instance =
   std::make_unique<PeriodicJobController>();

  struct timeval tv = {5, 2};
  
  /* Try addition when the container is empty */
  auto id1 = ptr_periodic_class_controller_instance->addJob(tv);
  auto id2 = ptr_periodic_class_controller_instance->addJob(tv);
  BOOST_CHECK( !id1. empty() );
  BOOST_CHECK( !id2. empty() );

  /* Arbitrarily removed job's ID should be one of the added
  job IDs */
  auto some_deleted_job_id1 = 
    ptr_periodic_class_controller_instance
    ->removeAnArbitraryJob();
  BOOST_CHECK( some_deleted_job_id1 == id1 ||
               some_deleted_job_id1 == id2);

  /* Try addition when the container is not empty*/
  auto id3 = ptr_periodic_class_controller_instance->addJob(tv);      
  BOOST_CHECK( !id3. empty() );

  /*id3 exists in the container, deletion should be successful*/
  BOOST_CHECK( 
    ptr_periodic_class_controller_instance->removeJob(id3) );

  /* STATUS: There is only id1 and id2 left in the container.
  */

  auto some_deleted_job_id2 = 
    ptr_periodic_class_controller_instance
    ->removeAnArbitraryJob();

  /* There is only one element left and we know that if it is
  not id1, it is id2 and vice versa */
  if (some_deleted_job_id1 == id1)
    BOOST_CHECK( some_deleted_job_id2 == id2);
  else
    BOOST_CHECK( some_deleted_job_id2 == id1);

  /* STATUS: Now the container should be empty, try removing an
  element. */

  auto some_deleted_job_id3 = 
    ptr_periodic_class_controller_instance
    ->removeAnArbitraryJob();
  BOOST_CHECK( some_deleted_job_id3.empty() );

  BOOST_CHECK(
    !ptr_periodic_class_controller_instance->removeJob(id1));
  BOOST_CHECK(
    !ptr_periodic_class_controller_instance->removeJob(id1));
  BOOST_CHECK(
    !ptr_periodic_class_controller_instance->removeJob(id2));

}

/**
 * @brief Tests the behaviour of addJob, removeJob and
 * removeAnArbitraryJob for different kinds of scenarios.
 *
 * Check whether remove methods really remove and addJob method
 * really add PeriodicJobs correctly. To test these, the
 * internal queue is directly accessed and the contents are
 * checked.
 *
 */
BOOST_AUTO_TEST_CASE (ADDITION_REMOVAL_BEHAVIOUR_TEST)
{
  auto ptr_periodic_class_controller_instance =
   std::make_shared<PeriodicJobController>();

  struct timeval tv = {5, 2};
  auto id1 = ptr_periodic_class_controller_instance->addJob(tv);
  auto id2 = ptr_periodic_class_controller_instance->addJob(tv);

  const auto& active_jobs = 
    PeriodicJobControllerFriend::get_active_jobs
      (ptr_periodic_class_controller_instance);

  auto job1_itr = active_jobs.find(id1);
  auto job2_itr = active_jobs.find(id2);
      
  /* Check if the jobs are really added to the container */
  BOOST_CHECK(job1_itr != active_jobs.end() );
  BOOST_CHECK(job2_itr != active_jobs.end() );

  auto deleted_id = ptr_periodic_class_controller_instance
    ->removeAnArbitraryJob();

  /* Update the iterators and make sure the correct job is
  deleted */
  job1_itr = active_jobs.find(id1);
  job2_itr = active_jobs.find(id2);
  if(deleted_id == id1)
  {
    BOOST_CHECK(job1_itr == active_jobs.end() );
    BOOST_CHECK(job2_itr != active_jobs.end() );

    ptr_periodic_class_controller_instance
      ->removeJob(id2);
  }
  else
  {
    BOOST_CHECK(job2_itr == active_jobs.end() );
    BOOST_CHECK(job1_itr != active_jobs.end() );

    ptr_periodic_class_controller_instance
      ->removeJob(id1);
  }

  /* Check if last removeJob call deleted the right job. */
  BOOST_CHECK(active_jobs.empty());
}

BOOST_AUTO_TEST_CASE (changePeriod_RETURNVALUE_TEST)
{
  auto ptr_periodic_class_controller_instance =
   std::make_unique<PeriodicJobController>();

  /* Check if trying to change the period of non existing
  PeriodicJobs fails. It should fail*/
  struct timeval tv = {5, 0};
  BOOST_CHECK( 
    !ptr_periodic_class_controller_instance
    ->changePeriod("not exist", tv)
    );
  tv = {3, 1};
  BOOST_CHECK( 
    !ptr_periodic_class_controller_instance
    ->changePeriod("", tv)
    );

  /* id1 is an existing job, period change should work */
  auto id1 = ptr_periodic_class_controller_instance->addJob(tv);
  BOOST_CHECK( 
    ptr_periodic_class_controller_instance
    ->changePeriod(id1, tv)
    );

  tv = {9, 7};
  BOOST_CHECK( 
    ptr_periodic_class_controller_instance
    ->changePeriod(id1, tv)
    );

}

/**
 * @brief These test cases are prepared for checking whether
 * changing the period ofa job really changes this job's
 * period to the desired value.
 *
 * To test, the internal queue of PeriodicJobController is
 * directly accessed and the contents are checked.
 *
 */
BOOST_AUTO_TEST_CASE (changePeriod_BEHAVIOUR_TEST)
{
  auto ptr_periodic_class_controller_instance =
   std::make_shared<PeriodicJobController>();

  struct timeval tv = {5, 2};
  auto id1 = ptr_periodic_class_controller_instance->addJob(tv);

  const auto& active_jobs = 
    PeriodicJobControllerFriend::get_active_jobs
      (ptr_periodic_class_controller_instance);

  const auto job_itr = active_jobs.find(id1);

  /* Check if the added job has the correct period value */
  const auto& period = job_itr->second->get_period(); 
  BOOST_REQUIRE_EQUAL(period.tv_sec, tv.tv_sec);
  BOOST_REQUIRE_EQUAL(period.tv_usec, tv.tv_usec);

  /* Change the period*/
  struct timeval new_tv = {3, 4};
  ptr_periodic_class_controller_instance
  ->changePeriod(id1, new_tv);

  /* Check if the period now equals to the new value*/
  BOOST_REQUIRE_EQUAL(period.tv_sec, new_tv.tv_sec);
  BOOST_REQUIRE_EQUAL(period.tv_usec, new_tv.tv_usec); 
}

/**
 * @brief Give warnings if some of the assumptions made about
 * the target architecture or development environment is wrong
 * etc.
 *
 * Set the enviroment variable "BOOST_TEST_LOG_LEVEL" to warning
 * (i.e. BOOST_TEST_LOG_LEVEL=warning) to see the BOOST_WARN
 * outputs when running the tests.
 *
 */
BOOST_AUTO_TEST_CASE (PLATFORM_TESTS)
{
  /* #of bits per byte might not be 8-bits always (though
  rarely). We might want to check that as well. CHAR_BITS
  expands to __CHAR_BITS__ in limits.h */
  BOOST_WARN(CHAR_BIT == 8);

  /* If unsigned size is less than 4 and say we want to store a
  IPv6 Jumbogram payload in an unsigned value thinking it can
  store 32-bit values, we might want to get warned about that.
  */
  BOOST_WARN( sizeof(unsigned) >= 4 );

}

BOOST_AUTO_TEST_SUITE_END()