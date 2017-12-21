cmake_minimum_required(VERSION 2.6)
project(aeio)

set(DEBUG "true")
if (DEBUG)
    add_definitions(-DDEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif()

include_directories(./)
set(TIO_SRC ae.c adlist.c anet.c zmalloc.c common.c lfq.c)
#add_executable(tio main.c ${TIO_SRC})#add_executable(test_zlib zlib_test.c message_zlib.c)
add_library(aeio STATIC ${TIO_SRC})#add_executable(test_zlib zlib_test.c message_zlib.c)

#target_link_libraries(test_zlib z)

#install(TARGETS tcp_client RUNTIME DESTINATION bin)
