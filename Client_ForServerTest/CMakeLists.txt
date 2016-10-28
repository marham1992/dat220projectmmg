cmake_minimum_required(VERSION 3.6)
project(Client_ForServerTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Client/Client.cpp Client/Client.h Client/Client_Main.cpp Shared/Main.cpp Shared/NetworkDefinitions.h Shared/PacketTypes.h Shared/PacketTypes.cpp)
add_executable(Client_ForServerTest ${SOURCE_FILES})

target_link_libraries(Client_ForServerTest sfml-graphics sfml-window sfml-system sfml-network)