cmake_minimum_required(VERSION 3.0)

project(duo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

#add_subdirectory(base)

add_executable(duo main.cc)

#target_link_libraries(duo duo_base pthread)