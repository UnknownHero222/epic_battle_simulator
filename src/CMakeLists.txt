add_executable(TotalBattleSimulator
    main.cpp
    Simulator.cpp
)

add_subdirectory(IO)

target_link_libraries(TotalBattleSimulator PRIVATE IO)

target_include_directories(TotalBattleSimulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/IO
)

target_compile_options(TotalBattleSimulator PRIVATE -Wno-unknown-pragmas)
