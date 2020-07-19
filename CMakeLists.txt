cmake_minimum_required(VERSION 3.16)
project(untitled)

set(CMAKE_CXX_STANDARD 17)


if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")
    #set(CMAKE_CXX_FLAGS /O2)
endif(WIN32)


find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)
add_executable(untitled src/main.cpp src/image/bitmap.h src/genetic_algorithm.h src/figures.h src/image/image.h src/image/sfml_image.h src/sorted_container.h src/timeit.h src/individual.h)
target_link_libraries(untitled sfml-graphics)


