cmake_minimum_required(VERSION 3.27)
project(total_battle_sim_test)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
add_executable(sw_battle_test ${SOURCES})

target_include_directories(sw_battle_test PUBLIC src/)

