cmake_minimum_required(VERSION 3.16)
project(untitled)

set(CMAKE_CXX_STANDARD 17)


if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")
    #set(CMAKE_CXX_FLAGS /O2)
else()
    set(CMAKE_CXX_FLAGS "-pthread")
endif(WIN32)


find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)
add_executable(untitled src/main.cpp src/image/bitmap.h src/genetic_algorithm/genetic_algorithm.h src/utils/shape.h src/image/image.h src/image/sfml_image_wrapper.h src/utils/container.h src/utils/timer.h src/image/arash_partow_bitmap_wrapper.h src/genetic_algorithm/individual.cpp src/image/bitmap.cpp src/image/sfml_image_wrapper.cpp src/image/arash_partow_bitmap_wrapper.cpp)
target_link_libraries(untitled sfml-graphics)


