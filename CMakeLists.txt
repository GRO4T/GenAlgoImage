cmake_minimum_required(VERSION 3.16)
project(untitled)

set(CMAKE_CXX_STANDARD 11)



if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")
endif(WIN32)


find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)
add_executable(untitled main.cpp bitmap.h genetic_algorithm.h figures.h)
target_link_libraries(untitled sfml-graphics)


