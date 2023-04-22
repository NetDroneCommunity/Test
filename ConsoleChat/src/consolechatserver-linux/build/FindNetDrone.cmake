#
# NetDrone Engine
# Copyright © 2022 Origin Studio Inc.
#

#
# cmake ../build -DGLOBAL_INCLUDE_DIR=/usr/include
#
if(GLOBAL_INCLUDE_DIR)
	find_file(NETDRONE_INCLUDE_DIR CommonHeader.h
	${GLOBAL_INCLUDE_DIR}/netdrone
	)
else(GLOBAL_INCLUDE_DIR)
	find_file(NETDRONE_INCLUDE_DIR CommonHeader.h
	${PROJECT_ROOT}/../../include/netdrone
	/usr/include/netdrone
	/usr/local/include/netdrone
	/opt/local/include/netdrone
	)
endif(GLOBAL_INCLUDE_DIR)

#
# cmake ../build -DGLOBAL_LIBRARY_DIR=/usr/lib
#
if(GLOBAL_LIBRARY_DIR)
	find_library(NETDRONE_LIBRARY_DIR netdrone
	${GLOBAL_LIBRARY_DIR}
	)
else(GLOBAL_LIBRARY_DIR)
	find_library(NETDRONE_LIBRARY_DIR netdrone_ccu30
	${PROJECT_ROOT}/../../lib
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	/usr/lib64
	/usr/local/lib64
	/opt/local/lib64
    )
endif(GLOBAL_LIBRARY_DIR)

message(STATUS "Project Root: ${PROJECT_ROOT}")

if(NETDRONE_INCLUDE_DIR AND NETDRONE_LIBRARY_DIR)
        if(PROJECT_LINK_LIBRARIES)
                set(PROJECT_LINK_LIBRARIES "${PROJECT_LINK_LIBRARIES} -lnetdrone_ccu30")
        else(PROJECT_LINK_LIBRARIES)
                set(PROJECT_LINK_LIBRARIES "-lnetdrone_ccu30")
        endif(PROJECT_LINK_LIBRARIES)

	if(NOT GLOBAL_INCLUDE_DIR)
		exec_program("${COMMAND_DIRNAME} ${NETDRONE_INCLUDE_DIR}" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
		exec_program("${COMMAND_DIRNAME} ${output_value}" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
		set(GLOBAL_INCLUDE_DIR "${output_value}")
		message(STATUS "Global Include Directory: ${GLOBAL_INCLUDE_DIR}")
	endif(NOT GLOBAL_INCLUDE_DIR)

	if(NOT GLOBAL_LIBRARY_DIR)
		exec_program("${COMMAND_DIRNAME} ${NETDRONE_LIBRARY_DIR}" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
		set(GLOBAL_LIBRARY_DIR "${output_value}")
		message(STATUS "Global Library Directory: ${GLOBAL_LIBRARY_DIR}")
	endif(NOT GLOBAL_LIBRARY_DIR)

	message(STATUS "NetDrone Include Directory: ${NETDRONE_INCLUDE_DIR}")
	message(STATUS "NetDrone Library Directory: ${NETDRONE_LIBRARY_DIR}")
endif(NETDRONE_INCLUDE_DIR AND NETDRONE_LIBRARY_DIR)

exec_program("${CMAKE_SOURCE_DIR}/getversion_netdrone.sh --major-version=${GLOBAL_INCLUDE_DIR}/netdrone/Version.h" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
set(NETDRONE_MAJOR_VERSION ${output_value})
exec_program("${CMAKE_SOURCE_DIR}/getversion_netdrone.sh --minor-version=${GLOBAL_INCLUDE_DIR}/netdrone/Version.h" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
set(NETDRONE_MINOR_VERSION ${output_value})
exec_program("${CMAKE_SOURCE_DIR}/getversion_netdrone.sh --patch-version=${GLOBAL_INCLUDE_DIR}/netdrone/Version.h" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
set(NETDRONE_PATCH_VERSION ${output_value})

set(NETDRONE_REQUIRED_VERSION "${NETDRONE_MAJOR_VERSION}.${NETDRONE_MINOR_VERSION}.${NETDRONE_PATCH_VERSION}")
message(STATUS "NetDrone Required Version: ${NETDRONE_REQUIRED_VERSION}")
