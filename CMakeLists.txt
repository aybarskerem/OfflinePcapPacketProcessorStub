cmake_minimum_required(VERSION 3.5.1)

# hide the variable which resides in CACHE intentionally
# set(CMAKE_CXX_COMPILER "/usr/bin/g++-9.3.0") 
set(CMAKE_CXX_COMPILER "/usr/bin/g++") 
project(offline_pcap_packet_processor)

# for std::threads
find_package(Threads REQUIRED) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#[[ 
I might add CONFIGURE_DEPENDS not to re-run 
cmake manully before a build if files are added/removed
file(GLOB PROJ_HEADER_FILES CONFIGURE_DEPENDS include/**.h) 
file(GLOB PROJ_SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp) 
]]
set(PROJ_MAIN_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
file(GLOB PROJ_SOURCE_FILES src/*.cpp) 
file(GLOB_RECURSE PROJ_HEADER_FILES include/*.h) 
list(REMOVE_ITEM PROJ_SOURCE_FILES ${PROJ_MAIN_FILE_PATH})

# child cmakes can use as well
set(INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include) 

# message VERBOSE not available with this cmake version
message(STATUS "PROJ_SOURCE_FILES (except for main.cpp) are:") 
foreach(file ${PROJ_SOURCE_FILES})
  message(STATUS "${file}")
endforeach()
message("")

message(STATUS "PROJ_HEADER_FILES are:")
foreach(file ${PROJ_HEADER_FILES})
  message(STATUS "${file}")
endforeach()
message("")

# add the header files to the add_executable call so that they
# are considered a part of the project (e.g. see them on IDEs
# etc.) message(STATUS "Creating a static lib periodic_job_lib
# out of PROJ_SOURCE_FILES...") add_library(periodic_job_lib
# STATIC ${PROJ_SOURCE_FILES} )
# target_include_directories(periodic_job_lib PUBLIC
# ${INCLUDE_FOLDER}) target_link_libraries(periodic_job_lib
# PUBLIC Threads::Threads )

# message(STATUS "Creating an executable called offline_pcap_packet_processor
# out of main.cpp...") add_executable (offline_pcap_packet_processor
# ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
# ${${PROJ_HEADER_FILES}})
# target_include_directories(offline_pcap_packet_processor PUBLIC
# ${INCLUDE_FOLDER} ) target_link_libraries(offline_pcap_packet_processor PUBLIC
# ${periodic_job_lib} Threads::Threads  ) # for std::threads
# and boost test

message(STATUS "Creating an executable called 
                offline_pcap_packet_processor out of source 
                files...")
add_executable  (offline_pcap_packet_processor 
                ${PROJ_MAIN_FILE_PATH} 
                ${PROJ_SOURCE_FILES} 
                ${PROJ_HEADER_FILES})
target_include_directories( offline_pcap_packet_processor PUBLIC 
                            ${INCLUDE_FOLDER} )

# for std::threads and boost test
target_link_libraries( offline_pcap_packet_processor PUBLIC 
                       ${periodic_job_lib} 
                       Threads::Threads ) 

add_subdirectory(test)
