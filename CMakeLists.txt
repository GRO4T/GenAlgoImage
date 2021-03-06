cmake_minimum_required(VERSION 3.16)
project(genetic-image)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE -O3)

if (UNIX)
    find_package(SFML 2.5 COMPONENTS graphics)
    include_directories(include)
    add_subdirectory(lib/rapidjson)
    include_directories(lib/rapidjson/include)

    file (GLOB sources
        "include/*.hpp"
        "src/*.cpp"
    )
    add_executable(genetic-image ${sources})
    target_link_libraries(genetic-image sfml-graphics)

    # copy /res folder
    add_custom_command(TARGET genetic-image POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:genetic-image>/res
    )

else()
    message(STATUS "This project is Linux only")
endif(UNIX)



