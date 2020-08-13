###############################################################################
# Build test applications
#
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#----------------------------------------------------------------------------
# Echo client targets
#
add_executable(EchoClient_so echoclient.cpp)
set_target_properties(EchoClient_so PROPERTIES OUTPUT_NAME echoclient_so)
target_link_libraries(EchoClient_so PRIVATE Socket_shared)
add_executable(EchoClient_a echoclient.cpp)
set_target_properties(EchoClient_a PROPERTIES OUTPUT_NAME echoclient_a)
target_link_libraries(EchoClient_a PRIVATE Socket_static)


#----------------------------------------------------------------------------
# Echo server targets
#
add_executable(EchoServer_so echoserver.cpp)
set_target_properties(EchoServer_so PROPERTIES OUTPUT_NAME echoserver_so)
target_link_libraries(EchoServer_so PRIVATE Socket_shared)
add_executable(EchoServer_a echoserver.cpp)
set_target_properties(EchoServer_a PROPERTIES OUTPUT_NAME echoserver_a)
target_link_libraries(EchoServer_a PRIVATE Socket_static)
