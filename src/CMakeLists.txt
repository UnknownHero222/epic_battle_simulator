cmake_minimum_required(VERSION 3.10)

project(EpicTotalBattleSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(IO)

add_executable(EpicTotalBattleSimulator main.cpp)

target_link_libraries(EpicTotalBattleSimulator PRIVATE IO)