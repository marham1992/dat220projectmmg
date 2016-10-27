cmake_minimum_required(VERSION 3.6)
project(chatinputtest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp chatbox.cpp chatbox.h textinputarea.cpp textinputarea.h basetextbox.cpp basetextbox.h textbox.cpp textbox.h)
add_executable(chatinputtest ${SOURCE_FILES})

target_link_libraries(chatinputtest sfml-graphics sfml-window sfml-system)