cmake_minimum_required(VERSION 2.8)

project(cppJson_test)
include_directories(header .)
aux_source_directory(source SRC_LIST)


add_library(cppJson ${SRC_LIST} )
add_executable(json_test test/main.cpp)
target_link_libraries(json_test cppJson)