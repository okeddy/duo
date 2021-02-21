cmake_minimum_required(VERSION 3.0)

project(duo_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

add_subdirectory(../base base.out)

#add_executable(duo_test TimeStamp_unittest.cc)
#add_executable(duo_test mainArgs_test.cc)
#add_executable(duo_test LogFile_test.cc)
add_executable(duo_test Logging_test.cc)

target_link_libraries(duo_test duo_base pthread)