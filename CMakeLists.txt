cmake_minimum_required(VERSION 3.16)
project(untitled)

set(CMAKE_CXX_STANDARD 17)



if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")
endif(WIN32)


find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)
add_executable(untitled src/main.cpp src/bitmap.h src/genetic_algorithm.h src/figures.h src/image.h src/sfml_image.h)
target_link_libraries(untitled sfml-graphics)


