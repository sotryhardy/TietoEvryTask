cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)

project(TietoEvryTask)

file(GLOB SRCS *.cpp)
add_executable(specific_grep ${SRCS})
