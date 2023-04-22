#
# NetDrone Engine
# Copyright © 2022 Origin Studio Inc.
#

find_file(PTHREAD_INCLUDE_DIR pthread.h
/usr/include
/usr/local/include
)

find_library(PTHREAD_LIBRARY_DIR pthread
/usr/lib
/usr/local/lib
/usr/lib64
/usr/local/lib64
)

if(PTHREAD_INCLUDE_DIR AND PTHREAD_LIBRARY_DIR)
	if(PROJECT_LINK_LIBRARIES)
		set(PROJECT_LINK_LIBRARIES "${PROJECT_LINK_LIBRARIES} -lpthread")
	else(PROJECT_LINK_LIBRARIES)
		set(PROJECT_LINK_LIBRARIES "-lpthread")
	endif(PROJECT_LINK_LIBRARIES)

	exec_program("${COMMAND_DIRNAME} ${PTHREAD_LIBRARY_DIR}" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
	set(PTHREAD_LIBRARY_DIR "${output_value}")

	message(STATUS "Posix Thread Include Directory: ${PTHREAD_INCLUDE_DIR}")
	message(STATUS "Posix Thread Library Directory: ${PTHREAD_LIBRARY_DIR}")
endif(PTHREAD_INCLUDE_DIR AND PTHREAD_LIBRARY_DIR)
