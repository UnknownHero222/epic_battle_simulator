add_executable(SimulatorTests unit_tests.cpp map_tests.cpp)

target_link_libraries(SimulatorTests
    IO
    GTest::gtest_main
)

target_include_directories(SimulatorTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/IO
)

include(GoogleTest)
gtest_discover_tests(SimulatorTests)

target_compile_options(SimulatorTests PRIVATE -Wno-unknown-pragmas)