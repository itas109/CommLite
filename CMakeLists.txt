cmake_minimum_required(VERSION 3.11)

project(commlite LANGUAGES CXX)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

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
	GIT_TAG v4.1.0 # 2020-10-10
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



IF (WIN32)
	FetchContent_Declare( # cmake 3.11 above
		ncurses
		GIT_REPOSITORY https://gitee.com/itas109/PDCurses
		GIT_TAG 3.9 # 2019-09-05
		USES_TERMINAL_DOWNLOAD TRUE
		)
		
	FetchContent_GetProperties(ncurses)
	if(NOT ncurses_POPULATED)
	# Fetch the content using previously declared details
	FetchContent_Populate(ncurses)
	endif()

	add_custom_target(compile_ncurses
		COMMAND mingw32-make  -f Makefile WIDE=Y
		COMMAND ${CMAKE_COMMAND} -E copy ${ncurses_SOURCE_DIR}/curses.h ${ncurses_BINARY_DIR}/include/curses.h
		COMMAND ${CMAKE_COMMAND} -E copy ${ncurses_SOURCE_DIR}/wincon/pdcurses.a ${ncurses_BINARY_DIR}/lib/libpdcurses.a
		COMMENT "compile and install pdcurses"
		WORKING_DIRECTORY ${ncurses_SOURCE_DIR}/wincon
	)
elseif(UNIX)
	FetchContent_Declare( # cmake 3.11 above
		ncurses
		GIT_REPOSITORY https://gitee.com/itas109/ncurses
		GIT_TAG v6.2 # 2020-02-12
		USES_TERMINAL_DOWNLOAD TRUE
		)
		
	FetchContent_GetProperties(ncurses)
	if(NOT ncurses_POPULATED)
	# Fetch the content using previously declared details
	FetchContent_Populate(ncurses)
	endif()

	add_custom_target(compile_ncurses
		COMMAND ./configure --prefix=${ncurses_BINARY_DIR}
		COMMAND make -j `grep -c ^processor /proc/cpuinfo`
		COMMAND make install
		COMMENT "configure, compile and install ncurses"
		WORKING_DIRECTORY ${ncurses_SOURCE_DIR}
	)
ENDIF ()


if(APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(FOUNDATION_LIBRARY Foundation)
endif()

include_directories(src) # tui.h
include_directories(${cserialport_SOURCE_DIR}/include) # CSerialPort headers

include_directories(${ncurses_BINARY_DIR}/include)
include_directories(${ncurses_BINARY_DIR}/include/ncurses)# tui.h
link_directories(${ncurses_BINARY_DIR}/lib)

file(GLOB_RECURSE COMMON_SOURCES ${cserialport_SOURCE_DIR}/src/SerialPort.cpp ${cserialport_SOURCE_DIR}/src/SerialPortBase.cpp ${cserialport_SOURCE_DIR}/src/SerialPortInfo.cpp ${cserialport_SOURCE_DIR}/src/SerialPortInfoBase.cpp)

IF (WIN32)
	file(GLOB_RECURSE OS_ABOUT_SOURCES ${cserialport_SOURCE_DIR}/src/SerialPortInfoWinBase.cpp ${cserialport_SOURCE_DIR}/src/SerialPortWinBase.cpp)
ELSEIF (UNIX)
	file(GLOB_RECURSE OS_ABOUT_SOURCES ${cserialport_SOURCE_DIR}/src/SerialPortInfoUnixBase.cpp ${cserialport_SOURCE_DIR}/src/SerialPortUnixBase.cpp)
ENDIF ()

add_executable( ${PROJECT_NAME} src/CommLite.cpp src/tui.cpp ${COMMON_SOURCES} ${OS_ABOUT_SOURCES})

add_dependencies(${PROJECT_NAME} compile_ncurses)

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