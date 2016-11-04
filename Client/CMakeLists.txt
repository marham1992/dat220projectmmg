cmake_minimum_required(VERSION 3.6)
project(Client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")

set(SOURCE_FILES main.cpp Game.cpp Game.h Window.cpp Window.h TextBox.cpp TextBox.h EventManager.cpp EventManager.h SharedContext.h State_Intro.cpp State_Intro.h BaseState.h StateManager.cpp StateManager.h State_MainMenu.cpp State_MainMenu.h TextInputArea.cpp TextInputArea.h BaseTextBox.cpp BaseTextBox.h ChatBox.cpp ChatBox.h)
add_executable(Client ${SOURCE_FILES})

target_link_libraries(Client sfml-graphics sfml-window sfml-system)