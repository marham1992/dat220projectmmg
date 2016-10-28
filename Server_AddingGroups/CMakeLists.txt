cmake_minimum_required(VERSION 3.6)
project(Server_AddingGroups)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Shared/Main.cpp Shared/NetworkDefinitions.h Shared/PacketTypes.h Shared/PacketTypes.cpp Server/Server.cpp Server/Server.h Server/Server_Main.cpp)
add_executable(Server_AddingGroups ${SOURCE_FILES})

target_link_libraries(Server_AddingGroups sfml-graphics sfml-window sfml-system sfml-network)