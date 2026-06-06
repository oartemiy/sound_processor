enable_testing()

find_package(Catch2 3 QUIET)
if(NOT Catch2_FOUND)
    message(STATUS "Catch2 not found, downloading via FetchContent...")
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.15.0
    )
    FetchContent_MakeAvailable(Catch2)
    if(TARGET Catch2::Catch2WithMain)
    endif()
endif()

add_executable(sound_processor_tests 
    # tests/test_arg_parser.cpp
    # tests/test_wav_file.cpp
    # tests/test_filter.cpp
    tests/test_filter_producers.cpp

    ${PARSER_SOURCES}
    ${WAVFILE_SOURCES}
    ${FILTER_SOURCES}
    ${PIPELINE_SOURCES}
    ${CMDLINEARGS2PIPELINECONVERTER_SOURCES}
    ${FILTERPRODUCERS_SOURCES}
    ${APPLICATION_SOURCES}
)

target_link_libraries(sound_processor_tests PRIVATE project_options Catch2::Catch2WithMain)
target_include_directories(sound_processor_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(Catch)
catch_discover_tests(sound_processor_tests)