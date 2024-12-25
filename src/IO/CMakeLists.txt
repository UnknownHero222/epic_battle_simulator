add_library(IO STATIC
    Commands/CreateMap.hpp
    Events/MapCreated.hpp

    System/CommandParser.cpp
    System/CommandParser.hpp
    System/EventLog.hpp
    System/PrintDebug.hpp

    Core/Map.cpp
    Core/Map.hpp
    Core/MapVisitor.hpp
    Core/UnitVisitor.hpp

    Core/Units/Unit.cpp
    Core/Units/Unit.hpp
    Core/Units/Swordsman.cpp
    Core/Units/Swordsman.hpp
    Core/Units/Hunter.cpp
    Core/Units/Hunter.hpp
    Core/Units/Tower.cpp
    Core/Units/Griffon.cpp
)

target_include_directories(IO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(IO PRIVATE -Wno-unknown-pragmas)
