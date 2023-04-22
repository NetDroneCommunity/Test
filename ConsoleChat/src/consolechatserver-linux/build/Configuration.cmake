#
# NetDrone Engine
# Copyright © 2022 Origin Studio Inc.
#

if(NOT PROJECT_TITLE_NAME)
    set(PROJECT_PROGRAM_NAME "${PROJECT_NAME}")
    set(PACKAGE_NAME "${PROJECT_NAME}")
else(NOT PROJECT_TITLE_NAME)
    set(PROJECT_PROGRAM_NAME "${PROJECT_TITLE_NAME}_${PROJECT_NAME}")
    set(PACKAGE_NAME "${PROJECT_TITLE_NAME}_${PROJECT_NAME}")
endif(NOT PROJECT_TITLE_NAME)

set(PROJECT_PROGRAM_VERSION "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION}")
set(PACKAGE_VERSION "${PROJECT_PROGRAM_VERSION}")
set(PACKAGE_STRING "${PROJECT_PROGRAM_NAME} ${PROJECT_PROGRAM_VERSION}")

#
# Project Version
#
message(STATUS "Project Program Version: ${PROJECT_PROGRAM_VERSION}")

#
# Directory Setting
#
find_program(COMMAND_DIRNAME NAMES "dirname")
if(NOT COMMAND_DIRNAME)
    message(FATAL_ERROR "dirname is not found")
endif(NOT COMMAND_DIRNAME)

exec_program("${COMMAND_DIRNAME} ${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
set(PROJECT_HEADERS_DIR "${output_value}/src")
set(PROJECT_SOURCES_DIR "${PROJECT_HEADERS_DIR}")
set(PROJECT_BUILD_DIR "${output_value}/build")
set(PROJECT_ROOT "${output_value}")

#
# Build Time Setting
#
find_program(COMMAND_DATE NAMES "date")
if(NOT COMMAND_DATE)
    message(FATAL_ERROR "date is not found")
endif(NOT COMMAND_DATE)

exec_program("${COMMAND_DATE} +'%D %R%z'" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
set(BUILD_TIME "${output_value}")

#
# cmake ../build -DCMAKE_ARCH=x84
# cmake ../build -DCMAKE_ARCH=x86_64
#
if(CMAKE_ARCH MATCHES "x86_64")
    message(STATUS "system is x86_64")
    set(LIB_SUFFIX "64" CACHE STRING "Define suffix of directory name (64)")
    set(CXX_FLAGS_OPTION_ADD "-m64" CACHE STRING "Define option of compiler (64)")
else(CMAKE_ARCH MATCHES "x86_64")
    message(STATUS "system is x86")
    set(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32)")
    set(CXX_FLAGS_OPTION_ADD "" CACHE STRING "Define option of compiler (32)")
endif(CMAKE_ARCH MATCHES "x86_64")

#
# cmake ../build -DCMAKE_INSTALL_PREFIX=/usr
#
set(PROJECT_PROGRAM_DIR "${CMAKE_INSTALL_PREFIX}/bin")

message(STATUS "Install Program Directory: ${PROJECT_PROGRAM_DIR}")

#
# cmake ../build -DCMAKE_BUILD_TYPE=Debug
# cmake ../build -DCMAKE_BUILD_TYPE=Release
#
if(CMAKE_BUILD_TYPE MATCHES "Debug$")
    add_definitions(-D_DEBUG -D_BUILD_H -D_REENTRANT -D_THREAD)
    message(STATUS "Build Type: Debug")
endif(CMAKE_BUILD_TYPE MATCHES "Debug$")

if(CMAKE_BUILD_TYPE MATCHES "Release$")
    add_definitions(-D_BUILD_H -D_REENTRANT -D_THREAD)
    message(STATUS "Build Type: Release")
endif(CMAKE_BUILD_TYPE MATCHES "Release$")

if((CMAKE_CXX_COMPILER_VERSION MATCHES "^11\\.") OR (CMAKE_CXX_COMPILER_VERSION MATCHES "^12\\."))
	add_definitions(-D_SCHED)
endif()

#
# Flags Setting
#
if((CMAKE_CXX_COMPILER_VERSION MATCHES "^[7-9]\\.") OR (CMAKE_CXX_COMPILER_VERSION MATCHES "^10\\.") OR (CMAKE_CXX_COMPILER_VERSION MATCHES "^11\\.") OR (CMAKE_CXX_COMPILER_VERSION MATCHES "^12\\."))
	#set(CXX_FLAGS_OPTION_DEL "-Wno-class-memaccess -Wno-literal-suffix -Wno-format-truncation -Wno-stringop-truncation -Wno-array-bounds -Wno-unused-but-set-variable -Wno-memset-elt-size" CACHE STRING "")
	set(CXX_FLAGS_OPTION_DEL "-Wno-class-memaccess -Wno-format-truncation -Wno-stringop-truncation -Wno-array-bounds -Wno-unused-but-set-variable -Wno-memset-elt-size" CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-p ${CXX_FLAGS_OPTION_ADD} -fPIC -fstack-protector -funroll-all-loops -g3 -O2 -Wall ${CXX_FLAGS_OPTION_DEL} -rdynamic" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${CXX_FLAGS_OPTION_ADD} -s -fPIC -fstack-protector -funroll-all-loops -g0 -O2 -Wall ${CXX_FLAGS_OPTION_DEL} -rdynamic" CACHE STRING "" FORCE)

#message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
#message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

#
# Find Package
#
find_package(Thread REQUIRED)
find_package(NetDrone REQUIRED)

#
# Uninstall Setting
#
configure_file("${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake")

#
# Distribute Setting
#
add_custom_target(dist "${CMAKE_SOURCE_DIR}/makedist.sh" --work-path="${CMAKE_BINARY_DIR}" --build-path="${PROJECT_BUILD_DIR}" --src-path="${PROJECT_SOURCES_DIR}" --package-name="${PROJECT_PROGRAM_NAME}-${PROJECT_PROGRAM_VERSION}")

#
# Build Setting
#
configure_file("${CMAKE_SOURCE_DIR}/build.h.cmake.in" "${CMAKE_SOURCE_DIR}/build.h")

#
# Spec Setting
#
configure_file("${CMAKE_SOURCE_DIR}/rpmbuild.spec.cmake.in" "${CMAKE_SOURCE_DIR}/${PROJECT_PROGRAM_NAME}.spec")

#
# Include Setting
#
if(GLOBAL_INCLUDE_DIR)
    include_directories(${GLOBAL_INCLUDE_DIR} ${PROJECT_HEADERS_DIR} ${PROJECT_BUILD_DIR})
else(GLOBAL_INCLUDE_DIR)
    include_directories(${PROJECT_HEADERS_DIR} ${PROJECT_BUILD_DIR})
endif(GLOBAL_INCLUDE_DIR)

#
# Library Setting
#
link_directories(/usr/lib64)

if(GLOBAL_LIBRARY_DIR)
    link_directories(${GLOBAL_LIBRARY_DIR})
endif(GLOBAL_LIBRARY_DIR)
