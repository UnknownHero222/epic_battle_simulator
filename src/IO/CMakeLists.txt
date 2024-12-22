set(target IO)

add_library(${target} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Commands/CreateMap.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Events/MapCreated.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/System/CommandParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/CommandParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/EventLog.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/PrintDebug.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Map.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/MapVisitor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/UnitVisitor.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Units/Unit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Units/Unit.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Units/Swordsman.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Units/Swordsman.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Units/Hunter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Units/Hunter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Units/Tower.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Units/Griffon.hpp
)

target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(${target} PRIVATE -Wno-unknown-pragmas)
