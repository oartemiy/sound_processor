cmake_minimum_required(VERSION 3.24.0)

project(sound_processor VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(Catch2 3 REQUIRED)

set(PARSER_SOURCES src/ArgsParser/ArgsParser.cpp)

add_executable(sound_processor_tests 
    tests/test_arg_parser.cpp
    ${PARSER_SOURCES}
)


add_executable(sound_processor 
    src/main.cpp
    ${PARSER_SOURCES}

)

target_link_libraries(sound_processor_tests PRIVATE Catch2::Catch2WithMain)

target_include_directories(sound_processor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(sound_processor_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(Catch)
catch_discover_tests(sound_processor_tests)