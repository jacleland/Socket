###############################################################################
# Inet::Socket library
#
cmake_minimum_required (VERSION 3.10)
#set(CMAKE_VERBOSE_MAKEFILE 1)

#------------------------------------------------------------------------------
# Project name and config
#
project(Socket)

###############################################################################
# Define include directories to include the directory of the current module
# and src for unit tests, etc.,
#
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
)

#----------------------------------------------------------
# Locate CxxTest
#find_package(CxxTest REQUIRED)
find_package(CxxTest)



#----------------------------------------------------------
#
set(DEPENDENCY_LIST
	"python-dev (>= 2.7.5)"
)

#----------------------------------------------------------
# Source files for socket library
#
set(LIBRARY_SOURCE_FILES
	src/Address.cpp
	src/AbstractSocket.cpp
	src/ConnectionEndpoint.cpp
	src/ClientSocket.cpp
	src/ServerSocket.cpp
	src/NetStream.cpp
)

###############################################################################
# Library targets
#

# Shared library target
add_library(Socket_shared SHARED ${LIBRARY_SOURCE_FILES})
set_property(TARGET Socket_shared PROPERTY POSITION_INDEPENDENT_CODE 1)
set_target_properties(Socket_shared PROPERTIES OUTPUT_NAME Socket)

# Static library target
add_library(Socket_static STATIC ${LIBRARY_SOURCE_FILES})
set_target_properties(Socket_static PROPERTIES OUTPUT_NAME Socket)

###############################################################################
# Add test application subdirectory to build
#
add_subdirectory(testapp)

###############################################################################
# If CxxTest is installed, build unit tests for CTest
#
if(CXXTEST_FOUND)
	include(CTest)
	add_subdirectory(tests)
endif()

###############################################################################
# Install targets for libSocket.a and libSocket.so
#
install(
	TARGETS
		Socket_shared
		Socket_static
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/
	DESTINATION include/Inet
	FILES_MATCHING PATTERN "*.h*"
)
