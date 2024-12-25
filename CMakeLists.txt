cmake_minimum_required(VERSION 3.20)
project(total_battle_sim_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
