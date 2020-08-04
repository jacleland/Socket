#----------------------------------------------------------
# Add directories for test build
#
include_directories(${CXXTEST_INCLUDE_DIR})
link_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#----------------------------------------------------------
# Enable testing
enable_testing()

#----------------------------------------------------------
# Test NetStream encode/decode primatives
#
CXXTEST_ADD_TEST(StreamPrimatives_a
	StreamPrimatives.cpp ${CMAKE_CURRENT_SOURCE_DIR}/StreamPrimatives.h
)
target_link_libraries(StreamPrimatives_a PUBLIC Socket_static)

# Using shared library
CXXTEST_ADD_TEST(StreamPrimatives_so
	StreamPrimatives.cpp ${CMAKE_CURRENT_SOURCE_DIR}/StreamPrimatives.h
)
target_link_libraries(StreamPrimatives_so PUBLIC Socket_shared)

#----------------------------------------------------------
# Test NetStream encode/decode string data
#
CXXTEST_ADD_TEST(StreamString_a
	StreamString.cpp ${CMAKE_CURRENT_SOURCE_DIR}/StreamString.h
)
target_link_libraries(StreamString_a PUBLIC Socket_static)

# Using shared library
CXXTEST_ADD_TEST(StreamString_so
	StreamString.cpp ${CMAKE_CURRENT_SOURCE_DIR}/StreamString.h
)
target_link_libraries(StreamString_so PUBLIC Socket_shared)
