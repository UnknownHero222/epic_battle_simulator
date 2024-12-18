set(target IO)

add_library(${target} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Commands/CreateMap.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Events/MapCreated.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/CommandParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/CommandParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/EventLog.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/PrintDebug.hpp
)

target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(${target} PRIVATE -Wno-unknown-pragmas)
