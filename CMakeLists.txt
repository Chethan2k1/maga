cmake_minimum_required(VERSION 3.1)
project(maga VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(include_files
    ${CMAKE_SOURCE_DIR}/includes/server.hpp
    ${CMAKE_SOURCE_DIR}/includes/connection.hpp
    ${CMAKE_SOURCE_DIR}/includes/error.hpp
)

set(src_files
    ${CMAKE_SOURCE_DIR}/src/server.cpp
    ${CMAKE_SOURCE_DIR}/src/connection.cpp
)

add_executable(server main.cpp ${include_files} ${src_files})
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/includes)

#dependencies

find_package(Threads REQUIRED)
add_library(boost INTERFACE)
if(APPLE)
    message(STATUS "This is an apple system")
    target_include_directories(boost INTERFACE /usr/local/include)
else()
    target_include_directories(boost INTERFACE /usr/include)
endif()
target_link_libraries(boost INTERFACE Threads::Threads)

target_link_libraries(server PUBLIC boost)
