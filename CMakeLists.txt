cmake_minimum_required(VERSION 3.16)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
add_executable(untitled main.cpp)
target_link_libraries(untitled sfml-graphics)


