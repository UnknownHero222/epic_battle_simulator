cmake_minimum_required(VERSION 3.10)

project(TotalBattleSimulator)

add_subdirectory(IO)

set(target TotalBattleSimulator)

add_executable(${target} Main.cpp)

target_link_libraries(${target} PRIVATE Core IO)

target_include_directories(${target} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/IO
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
