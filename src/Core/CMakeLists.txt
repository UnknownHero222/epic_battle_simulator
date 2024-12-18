set(target Core)

add_library(${target} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Event.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EventHandler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CreateMapHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MapCreatedHandler.cpp
)

target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(${target} PRIVATE -Wno-unknown-pragmas)