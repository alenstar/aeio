cmake_minimum_required(VERSION 2.8)
# project(flds C)


if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "Do not build in-source.\nPlease remove CMakeCache.txt and the CMakeFiles/ directory.\nThen: mkdir build ; cd build ; cmake .. ; make")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Profile Release Asan Ubsan." FORCE)
endif(NOT CMAKE_BUILD_TYPE)


if(BUILD_FOR_ARM)
    SET(CROSS_TOOLS_PATH /opt/crosstools/arm-2009q3/bin/arm-none-linux-gnueabi)
    SET(CROSS_ROOT_PATH /opt/crosstools/sys-root)

    # this one is important
    SET(CMAKE_SYSTEM_NAME Linux)
    #this one not so much
    SET(CMAKE_SYSTEM_VERSION 1)

    # specify the cross compiler
    SET(CMAKE_C_COMPILER   ${CROSS_TOOLS_PATH}-gcc)
    SET(CMAKE_CXX_COMPILER ${CROSS_TOOLS_PATH}-g++)

    # where is the target environment
    SET(CMAKE_FIND_ROOT_PATH  ${CROSS_ROOT_PATH})

    # search for programs in the build host directories
    SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    # for libraries and headers in the target directories
    SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif()

#option(BUILD_EXAMPLE "Build example" ON)

set(LFDS_LIST_ORDERED
        liblfds.h
        liblfds_internal.h
        lfds_porting_abstraction_layer_compiler.h
        lfds_porting_abstraction_layer_operating_system.h
        lfds_porting_abstraction_layer_processor.h
        lfds_list_addonly_singlylinked_ordered.h
        lfds_list_addonly_singlylinked_ordered_internal.h
        misc/lfds_misc_globals.c                misc/lfds_misc_query.c
        misc/lfds_misc.h                        misc/lfds_prng.h
        misc/lfds_misc_internal_backoff_init.c  misc/lfds_prng_init.c
        misc/lfds_misc_internal.h               misc/lfds_prng_internal.h
        lfds_list_addonly_singlylinked_ordered_cleanup.c
        lfds_list_addonly_singlylinked_ordered_get.c
        lfds_list_addonly_singlylinked_ordered_init.c
        lfds_list_addonly_singlylinked_ordered_insert.c
        lfds_list_addonly_singlylinked_ordered_query.c)

if(BUILD_LFDS_STATIC_LIB)
    #set(CMAKE_CXX_STANDARD 98)
    #set(CMAKE_C_STANDARD 99)
    add_library("${PROJECT_NAME}_static" STATIC ${LFDS_LIST_ORDERED})
    install(TARGETS "${PROJECT_NAME}_static" DESTINATION ${CMAKE_BINARY_DIR}/lib)
endif()

install(FILES ${PROJECT_SOURCE_DIR}/lfds_list_ordered.h DESTINATION ${CMAKE_BINARY_DIR}/include)
install(FILES ${PROJECT_SOURCE_DIR}/lfds_porting_abstraction_layer_compiler.h DESTINATION ${CMAKE_BINARY_DIR}/include)
install(FILES ${PROJECT_SOURCE_DIR}/lfds_porting_abstraction_layer_processor.h DESTINATION ${CMAKE_BINARY_DIR}/include)
install(FILES ${PROJECT_SOURCE_DIR}/lfds_porting_abstraction_layer_operating_system.h DESTINATION ${CMAKE_BINARY_DIR}/include)
install(FILES ${PROJECT_SOURCE_DIR}/lfds_list_addonly_singlylinked_ordered_internal.h DESTINATION ${CMAKE_BINARY_DIR}/include)
