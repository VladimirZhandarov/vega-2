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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

include(GenerateExportHeader)

link_directories(${CMAKE_SOURCE_DIR}/../bin)
set(VEGA2_LIBRARY vega2lib)

if(DEFINED NO_AES)
	add_definitions(-DNO_AES)
else()
	link_directories(/usr/local/ssl/lib)
	set(CRYPTO_LIBRARY ssl crypto)
endif()

EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
message(STATUS "Architecture: ${ARCHITECTURE}")

file(GLOB SOURCES *.cpp)

if(DEFINED NO_GUARDANT)
	add_definitions(-DNO_GUARDANT)
else()
	include_directories(../guardantlib/include)
	set(GUARDANT_LIBRARY grdapi)
	set(SOURCES ${SOURCES} ../guardantlib/grddongle.cpp)

if(${ARCHITECTURE} STREQUAL "x86_64")
	link_directories(${CMAKE_SOURCE_DIR}/../guardantlib/x86_64)
else()
	link_directories(${CMAKE_SOURCE_DIR}/../guardantlib/x86)
endif()

endif()

set(BOOST_COMPONENTS 
	thread
	system
	filesystem)
 
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

if(${ARCHITECTURE} STREQUAL "x86_64")
	message(STATUS "Compile with iANPR1.7/x64/libianpr1.7.so")
	link_directories(${CMAKE_SOURCE_DIR}/../iANPR1.7/x64)
	set(IARNP17_LIBRARY ianpr1.7)
elseif(${ARCHITECTURE} STREQUAL "i686")
	message(STATUS "Compile with iANPR1.7/x86/libianpr1.7.so")
	link_directories(${CMAKE_SOURCE_DIR}/../iANPR1.7/x86)
	set(IARNP17_LIBRARY ianpr1.7)
else()
	message(STATUS "Compile with iANPR1.7/arm/libianpr1.793.so")
	link_directories(${CMAKE_SOURCE_DIR}/../iANPR1.7/arm)
	set(IARNP17_LIBRARY ianpr1.793)

if(DEFINED USE_GPIOD)
	add_definitions(-DUSE_GPIOD)
else()
	add_definitions(-DUSE_PIFACE_DIGITAL)
	set(PIFACE_LIBRARY pifacedigital mcp23s17)
endif()

endif()

link_directories(/usr/lib/arm-linux-gnueabihf)

if(DEFINED USE_MONGODB)
	add_definitions(-DUSE_MONGODB)
	set(MONGOC_LIBRARY mongoc-1.0)
	set(BSON_LIBRARY bson-1.0)
endif()

find_package(OpenCV REQUIRED)

include_directories(../iANPR1.7)
include_directories(../iANPR1.7/include)
include_directories(../vega2lib)

add_executable(vega2app ${SOURCES})

target_link_libraries(vega2app ${VEGA2_LIBRARY} ${CRYPTO_LIBRARY} ${GUARDANT_LIBRARY} ${WIRING_PI_LIBRARY} ${PIFACE_LIBRARY} ${IARNP17_LIBRARY} ${BSON_LIBRARY} ${MONGOC_LIBRARY} ${OpenCV_LIBS} ${Boost_LIBRARIES})

INSTALL(TARGETS vega2app DESTINATION /opt/cerberus)

if(${ARCHITECTURE} STREQUAL "x86_64")
	INSTALL(FILES ${CMAKE_SOURCE_DIR}/../iANPR1.7/x64/libianpr1.7.so DESTINATION /usr/lib)
elseif(${ARCHITECTURE} STREQUAL "i686")
	INSTALL(FILES ${CMAKE_SOURCE_DIR}/../iANPR1.7/x86/libianpr1.7.so DESTINATION /usr/lib)
else()
	INSTALL(FILES ${CMAKE_SOURCE_DIR}/../iANPR1.7/arm/libianpr1.793.so DESTINATION /usr/lib)
endif()

if(DEFINED NO_AES AND DEFINED NO_GUARDANT)
	INSTALL(FILES ${CMAKE_SOURCE_DIR}/../iANPR1.7/ooosmit.key DESTINATION /opt/cerberus)
endif()

if(NOT DEFINED NO_GUARDANT)
	INSTALL(FILES ${CMAKE_SOURCE_DIR}/../guardantlib/etc/95-grdnt.rules DESTINATION /opt/cerberus)
	INSTALL(FILES ${CMAKE_SOURCE_DIR}/../guardantlib/etc/grdnt-install.sh DESTINATION /opt/cerberus)

if(${ARCHITECTURE} STREQUAL "x86_64")
	INSTALL(FILES ${CMAKE_SOURCE_DIR}/../guardantlib/x86_64/libgrdapi.so DESTINATION /usr/lib)
elseif(${ARCHITECTURE} STREQUAL "i686")
	INSTALL(FILES ${CMAKE_SOURCE_DIR}/../guardantlib/x86/libgrdapi.so DESTINATION /usr/lib)
endif()

endif()

INSTALL(FILES ${CMAKE_SOURCE_DIR}/install.sh DESTINATION /opt/cerberus)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/initd/vega2app DESTINATION /etc/init.d)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/initd/vega-proxy-app DESTINATION /etc/init.d)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/vega-proxy-1.0.1.jar DESTINATION /opt/cerberus)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/cam-vega-driver-1.0.1.jar DESTINATION /opt/cerberus/lib)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/commons-codec-1.9.jar DESTINATION /opt/cerberus/lib)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/commons-io-2.4.jar DESTINATION /opt/cerberus/lib)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/commons-logging-1.2.jar DESTINATION /opt/cerberus/lib)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/hamcrest-core-1.1.jar DESTINATION /opt/cerberus/lib)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/httpclient-4.5.2.jar DESTINATION /opt/cerberus/lib)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/httpcore-4.4.4.jar DESTINATION /opt/cerberus/lib)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/junit-4.10.jar DESTINATION /opt/cerberus/lib)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/log4j-1.2.15.jar DESTINATION /opt/cerberus/lib)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/protobuf-java-2.6.1.jar DESTINATION /opt/cerberus/lib)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/slf4j-api-1.7.21.jar DESTINATION /opt/cerberus/lib)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/../bin/lib/vega-proto-1.0.1.jar DESTINATION /opt/cerberus/lib)



set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/postinst")

if(${ARCHITECTURE} STREQUAL "i686")
INSTALL(FILES /usr/local/lib/libavcodec.so.55.52.102
	/usr/local/lib/libavcodec.so.55
	/usr/local/lib/libavcodec.so
	/usr/local/lib/libavformat.so.55.33.100
	/usr/local/lib/libavformat.so.55
	/usr/local/lib/libavformat.so
	/usr/local/lib/libavutil.so.52.66.100
	/usr/local/lib/libavutil.so
	/usr/local/lib/libavutil.so.52
	/usr/local/lib/libswscale.so.2
	/usr/local/lib/libswscale.so
	/usr/local/lib/libswscale.so.2.5.102
	/usr/local/lib/libx264.so.148
	/usr/local/lib/libx264.so
	DESTINATION /usr/lib)

INSTALL(FILES /usr/lib/libopencv_core.so.2.4
	/usr/lib/libopencv_core.so
	/usr/lib/libopencv_core.so.2.4.11
	/usr/lib/libopencv_highgui.so
	/usr/lib/libopencv_highgui.so.2.4
	/usr/lib/libopencv_highgui.so.2.4.11
	/usr/lib/libopencv_imgproc.so.2.4.11
	/usr/lib/libopencv_imgproc.so
	/usr/lib/libopencv_imgproc.so.2.4
	DESTINATION /usr/lib)
endif()

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "vega2app")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "7")
set(CPACK_PACKAGE_VERSION_PATCH "11")
set(CPACK_PACKAGE_DESCRIPTION "Vega-2 server")
set(CPACK_PACKAGE_CONTACT "Vladimir.Zhandarov@gmail.com")

include(CPack)
