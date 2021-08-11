cmake_minimum_required(VERSION 3.20)
project(Derivative)

set(CMAKE_CXX_STANDARD 20)

add_executable(Derivative main.cpp)

install(TARGETS Derivative DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)