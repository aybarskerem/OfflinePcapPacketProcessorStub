project(offline_pcap_packet_processor_test)

# To be able to use Boost unit test framework.
find_package(Boost 1.71.0 COMPONENTS unit_test_framework REQUIRED) 


message("include path is: ${INCLUDE_FOLDER}")

#[[ Do necessary registration for CTEST unit test.
call this so that tests added by add_test are run. ]] 
enable_testing()

message(STATUS "Creating an executable called 
                offline_pcap_packet_processor_test out of test 
                files...")
add_executable(offline_pcap_packet_processor_test 
               tests.cpp 
               ${PROJ_SOURCE_FILES} )
target_include_directories(offline_pcap_packet_processor_test 
                           PUBLIC 
                           ${INCLUDE_FOLDER} )

#[[ for std::threads and boost test
    To use boost standalone library compilation instead of 
    included, comment out the 
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
#]]
target_link_libraries(offline_pcap_packet_processor_test
                      PUBLIC 
                      Threads::Threads 
                      # ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      ) 
                      
add_test(NAME offline_pcap_packet_processor_test_name 
         COMMAND offline_pcap_packet_processor_test)
