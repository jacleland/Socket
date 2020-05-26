#----------------------------------------------------------
# Inet::Socket library
#
cmake_minimum_required (VERSION 3.10)
set(CMAKE_INSTALL_PREFIX ${HOME})
#set(CMAKE_VERBOSE_MAKEFILE 1)

#----------------------------------------------------------
# Project name and config
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
find_package(CxxTest REQUIRED)

#----------------------------------------------------------
#
set(DEPENDENCY_LIST
	"python-dev (>= 2.7.5)"
)

#----------------------------------------------------------
# Source files for socket library
#
set(
	LIBRARY_SOURCE_FILES
		src/Address.cpp
		src/AbstractSocket.cpp
		src/ConnectionEndpoint.cpp
		src/ClientSocket.cpp
		src/ServerSocket.cpp
)

#----------------------------------------------------------
# Library targets
#
add_library(Socket_shared SHARED ${LIBRARY_SOURCE_FILES})
set_property(TARGET Socket_shared PROPERTY POSITION_INDEPENDENT_CODE 1)
set_target_properties(Socket_shared PROPERTIES OUTPUT_NAME Socket)
add_library(Socket_static STATIC ${LIBRARY_SOURCE_FILES})
set_target_properties(Socket_static PROPERTIES OUTPUT_NAME Socket)

###############################################################################
# Define executable targets for socket echo client and server
#

#------------------------------------------------------------------------------
# Echo client target
#
set(CLIENT_SOURCE_FILES testapp/client.cpp)
add_executable(EchoClient ${CLIENT_SOURCE_FILES})
set_target_properties(EchoClient PROPERTIES OUTPUT_NAME echoclient)
target_link_libraries(EchoClient PRIVATE Socket_shared)

#----------------------------------------------------------
# Echo server target
#
set(SERVER_SOURCE_FILES testapp/server.cpp)
add_executable(EchoServer ${SERVER_SOURCE_FILES})
set_target_properties(EchoServer PROPERTIES OUTPUT_NAME echoserver)
target_link_libraries(EchoServer PRIVATE Socket_shared)

###############################################################################
# If CxxTest is installed, build unit tests
#
if(CXXTEST_FOUND)
# 	----------------------------------------------------------
# 	include_directories(
# 		${CMAKE_CURRENT_SOURCE_DIR}
# 		${CXXTEST_INCLUDE_DIR}
# 		${CMAKE_SOURCE_DIR}/src
# 	)
# 	enable_testing()
# 	CXXTEST_ADD_TEST(SocketTests SocketTests.cpp ${CMAKE_CURRENT_SOURCE_DIR}/tests/SocketTests.h)
# 	CXXTEST_ADD_TEST(AddressTests AddressTests.cpp ${CMAKE_CURRENT_SOURCE_DIR}/tests/AddressTests.h)
# 	target_link_libraries(AddressTests PUBLIC Socket_shared)
# 	target_link_libraries(SocketTests PUBLIC Socket_shared)
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
