add_executable(UnitTests tests.cpp)

target_link_libraries(UnitTests
    IO
    GTest::gtest_main
)

target_include_directories(UnitTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/IO
)

include(GoogleTest)
gtest_discover_tests(UnitTests)

target_compile_options(UnitTests PRIVATE -Wno-unknown-pragmas)
