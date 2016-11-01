cmake_minimum_required(VERSION 3.6)
project(Client_WithChatbox)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Shared/NetworkDefinitions.h Shared/PacketTypes.cpp Shared/PacketTypes.h Client/Client.cpp Client/Client.h Client/Client_Main.cpp Client/Game.cpp Client/Game.h Client/EventManager.cpp Client/EventManager.h Client/Window.cpp Client/Window.h Client/SharedContext.h Client/StateManager.cpp Client/StateManager.h Client/State_ChatTest.cpp Client/State_ChatTest.h Client/Textbox.cpp Client/Textbox.h Client/BaseState.h Client/TextInputArea.cpp Client/TextInputArea.h)
add_executable(Client_WithChatbox ${SOURCE_FILES})


target_link_libraries(Client_WithChatbox sfml-graphics sfml-window sfml-system sfml-network)