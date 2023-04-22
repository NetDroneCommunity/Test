#
# NetDrone Engine
# Copyright © 2022 Origin Studio Inc.
#

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(echoserver CXX)

#set(PROJECT_TITLE_NAME "example")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

exec_program("${CMAKE_SOURCE_DIR}/getversion_service.sh --major-version=${CMAKE_SOURCE_DIR}/../src/Version.h" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
set(PROJECT_MAJOR_VERSION ${output_value})
exec_program("${CMAKE_SOURCE_DIR}/getversion_service.sh --minor-version=${CMAKE_SOURCE_DIR}/../src/Version.h" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
set(PROJECT_MINOR_VERSION ${output_value})
exec_program("${CMAKE_SOURCE_DIR}/getversion_service.sh --patch-version=${CMAKE_SOURCE_DIR}/../src/Version.h" OUTPUT_VARIABLE output_value RETURN_VALUE return_value)
set(PROJECT_PATCH_VERSION ${output_value})

include(Configuration)

#
# Project Headers
#
set(PROJECT_HEADERS_DB
${PROJECT_HEADERS_DIR}/db/DbData.h
)

set(PROJECT_HEADERS_NET
${PROJECT_HEADERS_DIR}/net/data/ID.h
${PROJECT_HEADERS_DIR}/net/data/INFO.h
${PROJECT_HEADERS_DIR}/net/data/OTHER.h
${PROJECT_HEADERS_DIR}/net/data/ROOM.h
${PROJECT_HEADERS_DIR}/net/data/USER.h
${PROJECT_HEADERS_DIR}/net/ConnectorEx.h
${PROJECT_HEADERS_DIR}/net/Launcher.h
${PROJECT_HEADERS_DIR}/net/ListenerEx.h
${PROJECT_HEADERS_DIR}/net/NetIO.h
${PROJECT_HEADERS_DIR}/net/NetworkEx.h
${PROJECT_HEADERS_DIR}/net/NetMgr.h
)

set(PROJECT_HEADERS_UNIT
${PROJECT_HEADERS_DIR}/unit/Provider.h
${PROJECT_HEADERS_DIR}/unit/UnitData.h
${PROJECT_HEADERS_DIR}/unit/Unit.h
${PROJECT_HEADERS_DIR}/unit/Player.h
${PROJECT_HEADERS_DIR}/unit/PlayerEx.h
${PROJECT_HEADERS_DIR}/unit/UnitMgr.h
)

set(PROJECT_HEADERS_UTIL
${PROJECT_HEADERS_DIR}/util/CfgMgr.h
)

set(PROJECT_HEADERS_WORLD
${PROJECT_HEADERS_DIR}/world/Provider.h
${PROJECT_HEADERS_DIR}/world/Room.h
${PROJECT_HEADERS_DIR}/world/RoomEx.h
${PROJECT_HEADERS_DIR}/world/RoomHandler.h
${PROJECT_HEADERS_DIR}/world/RoomHandlerEx.h
${PROJECT_HEADERS_DIR}/world/ChannelMgr.h
${PROJECT_HEADERS_DIR}/world/ServerMgr.h
)

set(PROJECT_HEADERS
${PROJECT_HEADERS_DIR}/CommonHeader.h
${PROJECT_HEADERS_DIR}/Defines.h
${PROJECT_HEADERS_DIR}/Enums.h
${PROJECT_HEADERS_DIR}/Framework.h
${PROJECT_HEADERS_DIR}/GlobalVariable.h
${PROJECT_HEADERS_DIR}/Version.h
)

#
# Project Sources
#
set(PROJECT_SOURCES_DB
)

set(PROJECT_SOURCES_NET
${PROJECT_SOURCES_DIR}/net/ConnectorEx.cpp
${PROJECT_SOURCES_DIR}/net/ID.cpp
${PROJECT_SOURCES_DIR}/net/INFO.cpp
${PROJECT_SOURCES_DIR}/net/Launcher.cpp
${PROJECT_SOURCES_DIR}/net/ListenerEx.cpp
${PROJECT_SOURCES_DIR}/net/NetMgr.cpp
${PROJECT_SOURCES_DIR}/net/NetworkEx.cpp
${PROJECT_SOURCES_DIR}/net/ROOM.cpp
${PROJECT_SOURCES_DIR}/net/USER.cpp
)

set(PROJECT_SOURCES_UNIT
${PROJECT_SOURCES_DIR}/unit/Provider.cpp
${PROJECT_SOURCES_DIR}/unit/Unit.cpp
${PROJECT_SOURCES_DIR}/unit/Player.cpp
${PROJECT_SOURCES_DIR}/unit/PlayerEx.cpp
${PROJECT_SOURCES_DIR}/unit/UnitMgr.cpp
)

set(PROJECT_SOURCES_UTIL
${PROJECT_SOURCES_DIR}/util/CfgMgr.cpp
)

set(PROJECT_SOURCES_WORLD
${PROJECT_SOURCES_DIR}/world/Provider.cpp
${PROJECT_SOURCES_DIR}/world/Room.cpp
${PROJECT_SOURCES_DIR}/world/RoomEx.cpp
${PROJECT_SOURCES_DIR}/world/RoomHandler.cpp
${PROJECT_SOURCES_DIR}/world/RoomHandlerEx.cpp
${PROJECT_SOURCES_DIR}/world/ChannelMgr.cpp
${PROJECT_SOURCES_DIR}/world/ServerMgr.cpp
)

set(PROJECT_SOURCES
${PROJECT_SOURCES_DIR}/Framework.cpp
${PROJECT_SOURCES_DIR}/main.cpp
${PROJECT_SOURCES_DIR}/Version.cpp
)

#
# Program Setting
#
add_executable(${PROJECT_PROGRAM_NAME}
${PROJECT_SOURCES_DB}
${PROJECT_SOURCES_NET}
${PROJECT_SOURCES_UNIT}
${PROJECT_SOURCES_UTIL}
${PROJECT_SOURCES_WORLD}
${PROJECT_SOURCES}
)

set_target_properties(${PROJECT_PROGRAM_NAME} PROPERTIES
OUTPUT_NAME "${PROJECT_PROGRAM_NAME}-${PROJECT_PROGRAM_VERSION}"
)

if(PROJECT_LINK_LIBRARIES)
    target_link_libraries(${PROJECT_PROGRAM_NAME} ${PROJECT_LINK_LIBRARIES})
    message(STATUS "Linking: ${PROJECT_LINK_LIBRARIES}")
endif(PROJECT_LINK_LIBRARIES)

#
# Install Setting
#
install(PROGRAMS ${CMAKE_BINARY_DIR}/${PROJECT_PROGRAM_NAME}-${PROJECT_PROGRAM_VERSION} DESTINATION ${PROJECT_PROGRAM_DIR})
