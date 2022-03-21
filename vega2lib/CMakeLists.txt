cmake_minimum_required(VERSION 2.8)

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../bin)

include(GenerateExportHeader)

set(BOOST_COMPONENTS 
	thread
	system
	filesystem)
 
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

if(DEFINED USE_MONGODB)
	add_definitions(-DUSE_MONGODB)
	include_directories(/usr/include/libmongoc-1.0)
	include_directories(/usr/include/libbson-1.0)
endif()

if(DEFINED NO_AES)
	add_definitions(-DNO_AES)
else()
	include_directories(/usr/local/ssl/include)
endif()

if(DEFINED NO_GUARDANT)
	add_definitions(-DNO_GUARDANT)
else()
	include_directories(../guardantlib/include)
endif()
 
find_package(Threads)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(../iANPR1.7)
include_directories(../iANPR1.7/include)

EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
message(STATUS "Architecture: ${ARCHITECTURE}")

if(${ARCHITECTURE} STREQUAL "x86_64")
	message(STATUS "Compile with iANPR1.7/x64/libianpr1.7.so")
	link_directories(${CMAKE_SOURCE_DIR}/../iANPR1.7/x64)
elseif(${ARCHITECTURE} STREQUAL "i686")
	message(STATUS "Compile with iANPR1.7/x86/libianpr1.7.so")
	link_directories(${CMAKE_SOURCE_DIR}/../iANPR1.7/x86)
else()
	message(STATUS "Compile with iANPR1.7/arm/libianpr1.7.so")
	link_directories(${CMAKE_SOURCE_DIR}/../iANPR1.7/arm)

if(DEFINED USE_GPIOD)
	add_definitions(-DUSE_GPIOD)
else()
	add_definitions(-DUSE_PIFACE_DIGITAL)
	set(PIFACE_LIBRARY pifacedigital mcp23s17)
endif()

endif()

set(IARNP17_LIBRARY ianpr1.793)

file(GLOB SOURCES *.cpp)

add_library(vega2lib STATIC ${SOURCES})
GENERATE_EXPORT_HEADER(vega2lib
	BASE_NAME vega2lib
	EXPORT_MACRO_NAME VEGA2LIB_EXPORT
	EXPORT_FILE_NAME vega2libdef.h
	STATIC_DEFINE BUILD_VEGA2LIB_BUILT_AS_STATIC)

target_link_libraries(vega2lib ${PIFACE_LIBRARY} ${IARNP17_LIBRARY} ${Boost_LIBRARIES})
