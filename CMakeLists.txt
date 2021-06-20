cmake_minimum_required(VERSION 3.10)
project(maga VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(include_files
    ${CMAKE_SOURCE_DIR}/include/server.hpp
    ${CMAKE_SOURCE_DIR}/include/connection.hpp
    ${CMAKE_SOURCE_DIR}/include/error.hpp
    ${CMAKE_SOURCE_DIR}/include/parser.hpp
    ${CMAKE_SOURCE_DIR}/include/header.hpp
)

set(src_files
    # ${CMAKE_SOURCE_DIR}/src/server.cpp
    ${CMAKE_SOURCE_DIR}/src/connection.cpp
    ${CMAKE_SOURCE_DIR}/src/parser.cpp
    ${CMAKE_SOURCE_DIR}/src/header.cpp
)

add_definitions(-Wno-deprecated-declarations)
add_executable(server main.cpp ${include_files} ${src_files})
target_compile_options(server PUBLIC -Wall)
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src)

#dependencies

## magahttpparser
if(NOT DEFINED MAGAHTTPPARSER_ROOT)
    include(FetchContent)
    FetchContent_Declare(
        magahttpparser
        GIT_REPOSITORY https://github.com/Chethan2k1/magahttpparser.git
        GIT_TAG main
    )

    FetchContent_GetProperties(magahttpparser)
    if(NOT magahttpparser_POPULATED)
        FetchContent_Populate(magahttpparser)
        add_subdirectory("${magahttpparser_SOURCE_DIR}")
        set(ENABLE_TEST OFF CACHE BOOL "Don't build target test" FORCE)
        target_link_libraries(server PRIVATE mgparser)
    endif()
else()
    set(ENABLE_TEST OFF CACHE BOOL "Don't build target test" FORCE)
    add_subdirectory(${MAGAHTTPPARSER_ROOT} ${CMAKE_BINARY_DIR}/magahttpparser)
    target_link_libraries(server PRIVATE mgparser)
endif()

## boost

find_package(Threads REQUIRED)
target_link_libraries(server PUBLIC Threads::Threads)

if(DEFINED BOOST_INCLUDE_DIR)
    target_include_directories(server PUBLIC ${BOOST_INCLUDE_DIR})
else()
    find_package(Boost REQUIRED)
    message(STATUS ${BOOST_INCLUDE_DIR})
    target_include_directories(server PUBLIC ${BOOST_INCLUDE_DIR})
endif()

set_target_properties(server PROPERTIES COMPILE_OPTIONS "-I ${BOOST_INCLUDE_DIR}")

# formating
file(GLOB_RECURSE ALL_FILES ${src_files} ${include_files})

add_custom_target(
        format-maga
        COMMAND clang-format
        -style=file
        -i
        --verbose
        ${ALL_FILES}
)