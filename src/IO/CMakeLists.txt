cmake_minimum_required(VERSION 3.10)

# Set the project name
project(IO)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files
set(SOURCES
    Commands/CreateMap.hpp
    Commands/March.hpp
    Commands/SpawnHunter.hpp
    Commands/SpawnSwordsman.hpp
    Events/MapCreated.hpp
    Events/MarchEnded.hpp
    Events/MarchStarted.hpp
    Events/UnitAttacked.hpp
    Events/UnitDied.hpp
    Events/UnitMoved.hpp
    Events/UnitSpawned.hpp
    System/CommandParser.hpp
    System/CommandParser.cpp
    System/EventLog.hpp
    System/PrintDebug.hpp
    System/details/CommandParserVisitor.hpp
    System/details/PrintFieldVisitor.hpp
)

# Add the library
add_library(IO STATIC ${SOURCES})

# Include directories
target_include_directories(IO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})