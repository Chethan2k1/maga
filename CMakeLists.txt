cmake_minimum_required(VERSION 3.14)
project(maga VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

set(include_files
    ${CMAKE_SOURCE_DIR}/includes/server.hpp
    ${CMAKE_SOURCE_DIR}/includes/connection.hpp
    ${CMAKE_SOURCE_DIR}/includes/error.hpp
)

set(src_files
    # ${CMAKE_SOURCE_DIR}/src/server.cpp
    ${CMAKE_SOURCE_DIR}/src/connection.cpp
)

add_executable(server main.cpp ${include_files} ${src_files})
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/includes)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src)

#dependencies

## picohttpparser

include(FetchContent)
FetchContent_Declare(
    picohttpparser
    GIT_REPOSITORY https://github.com/h2o/picohttpparser.git
    GIT_TAG master
)

FetchContent_GetProperties(picohttpparser)
if(NOT picohttpparser_POPULATED)
    FetchContent_Populate(picohttpparser)
    add_library(picohttpparser STATIC
            "${picohttpparser_SOURCE_DIR}/picohttpparser.c"
            "${picohttpparser_SOURCE_DIR}/picohttpparser.h"
        )
    set_target_properties(picohttpparser PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(picohttpparser PUBLIC "${picohttpparser_SOURCE_DIR}")
    target_link_libraries(server PUBLIC picohttpparser)
endif()

## boost

find_package(Threads REQUIRED)
add_library(boost INTERFACE)
if(APPLE)
    target_include_directories(boost INTERFACE /usr/local/include)
else()
    target_include_directories(boost INTERFACE /usr/include)
endif()
target_link_libraries(boost INTERFACE Threads::Threads)

target_link_libraries(server PUBLIC boost)
