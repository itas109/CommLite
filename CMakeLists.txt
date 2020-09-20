cmake_minimum_required(VERSION 3.11)

project(commlite LANGUAGES CXX)

IF (WIN32)
 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
ENDIF ()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "CommLite CMake Info")
message(STATUS "=======================================================")
message(STATUS "    Operation System :  ${CMAKE_SYSTEM}")
message(STATUS "    CPU Architecture : ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "    Build Type : ${CMAKE_BUILD_TYPE}${CMAKE_CONFIGURATION_TYPES}")
message(STATUS "=======================================================")

include(FetchContent)

set(FETCHCONTENT_QUIET OFF CACHE INTERNAL "" FORCE)

FetchContent_Declare( # cmake 3.11 above
	CSerialPort
	GIT_REPOSITORY https://gitee.com/itas109/CSerialPort
	GIT_TAG f17b93655cbcdda8d8af1281fd70950d0a539e45 # 2020-09-20
	USES_TERMINAL_DOWNLOAD TRUE
	)

#FetchContent_MakeAvailable(CSerialPort) # cmake 3.14 above

# FetchContent_MakeAvailable
#message(STATUS "CSerialPort_SOURCE_DIR : ${CSerialPort_SOURCE_DIR}")
#message(STATUS "CSerialPort_BINARY_DIR : ${CSerialPort_BINARY_DIR}")

# only download CSerialPort
# Check if population has already been performed
FetchContent_GetProperties(CSerialPort)
if(NOT cserialport_POPULATED)
  # Fetch the content using previously declared details
  FetchContent_Populate(CSerialPort)
endif()

# FetchContent_GetProperties and FetchContent_Populate
message(STATUS "CSerialPort_SOURCE_DIR : ${cserialport_SOURCE_DIR}")
message(STATUS "CSerialPort_BINARY_DIR : ${cserialport_BINARY_DIR}")

if(APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(FOUNDATION_LIBRARY Foundation)
endif()

include_directories(.) # tui.h
include_directories(src) # tui.h
include_directories(${cserialport_SOURCE_DIR}/include) # CSerialPort headers

link_directories(.)

file(GLOB_RECURSE COMMON_SOURCES ${cserialport_SOURCE_DIR}/src/SerialPort.cpp ${cserialport_SOURCE_DIR}/src/SerialPortBase.cpp ${cserialport_SOURCE_DIR}/src/SerialPortInfo.cpp ${cserialport_SOURCE_DIR}/src/SerialPortInfoBase.cpp)

IF (WIN32)
	file(GLOB_RECURSE OS_ABOUT_SOURCES ${cserialport_SOURCE_DIR}/src/SerialPortInfoWinBase.cpp ${cserialport_SOURCE_DIR}/src/SerialPortWinBase.cpp)
ELSEIF (UNIX)
	file(GLOB_RECURSE OS_ABOUT_SOURCES ${cserialport_SOURCE_DIR}/src/SerialPortInfoUnixBase.cpp ${cserialport_SOURCE_DIR}/src/SerialPortUnixBase.cpp)
ENDIF ()

add_executable( ${PROJECT_NAME} src/CommLite.cpp src/tui.cpp ${COMMON_SOURCES} ${OS_ABOUT_SOURCES})

IF (WIN32)
	# for function availableFriendlyPorts
	# PDCurses-3.9\wincon : make -f Makefile WIDE=Y
	target_link_libraries( ${PROJECT_NAME} setupapi pdcurses libgcc.a libstdc++.a libpthread.a)
	# target_link_libraries( ${PROJECT_NAME} setupapi pdcurses)
elseif(APPLE)
    target_link_libraries( ${PROJECT_NAME} ${FOUNDATION_LIBRARY} ${IOKIT_LIBRARY} ncurses)
elseif(UNIX)
	target_link_libraries( ${PROJECT_NAME} pthread ncurses)
ENDIF ()