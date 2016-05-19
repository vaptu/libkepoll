cmake_minimum_required (VERSION 2.8)

add_definitions("-std=c++11")

set(PRODIR ${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PRODIR}/bin)

project (epoll_server)

include_directories(epoll_server include)


aux_source_directory(src DIR_SRCS)

add_executable(epoll_server ${DIR_SRCS})
