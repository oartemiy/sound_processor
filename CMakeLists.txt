cmake_minimum_required(VERSION 3.24.0)

project(sound_processor VERSION 0.1.0 LANGUAGES C CXX)
add_library(project_options INTERFACE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# enable_testing()

# find_package(Catch2 3 REQUIRED)
# if(NOT Catch2_FOUND)
#     message(STATUS "Catch2 not found, downloading via FetchContent...")
#     include(FetchContent)
#     FetchContent_Declare(
#         Catch2
#         GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#         GIT_TAG v3.15.0
#     )
#     FetchContent_MakeAvailable(Catch2)
#     if(TARGET Catch2::Catch2WithMain)
#     endif()
# endif()

set(PARSER_SOURCES 
    src/ArgsParser.cpp

    include/ArgsParser.h
)

set(WAVFILE_SOURCES 
    src/WavFile.cpp

    include/WavFile.h
)

set(FILTER_SOURCES 
    src/Filter/AmplFilter.cpp
    src/Filter/LowpassFilter.cpp
    src/Filter/NormalizeFilter.cpp
    src/Filter/SilenceFilter.cpp
    src/Filter/TimestretchFilter.cpp
    src/Filter/SinGenFilter.cpp
    src/Filter/AmGenFilter.cpp
    src/Filter/FmGenFilter.cpp

    include/Filter/AmplFilter.h
    include/Filter/LowpassFilter.h
    include/Filter/NormalizeFilter.h
    include/Filter/SilenceFilter.h
    include/Filter/TimestretchFilter.h
    include/Filter/SinGenFilter.h
    include/Filter/AmGenFilter.h
    include/Filter/FmGenFilter.h
)

set(PIPELINE_SOURCES
    src/Pipeline.cpp
    
    include/Pipeline.h
)

set(CMDLINEARGS2PIPELINECONVERTER_SOURCES
    src/CmdLineArgs2PipelineConverter.cpp
    
    include/CmdLineArgs2PipelineConverter.h    
)

set(FILTERPRODUCERS_SOURCES
    src/FilterProducers/amGenFilterCreator.cpp
    src/FilterProducers/amplFilterCreator.cpp
    src/FilterProducers/fmGenFilterCreator.cpp
    src/FilterProducers/lowpassFilterCreator.cpp
    src/FilterProducers/normalizeFilterCreator.cpp
    src/FilterProducers/silenceFilterCreator.cpp
    src/FilterProducers/sinGenFilterCreator.cpp
    src/FilterProducers/timestretchFilterCreator.cpp

    include/FilterProducers.h
)

set(APPLICATION_SOURCES
    src/Application.cpp

    include/Application.h
)

if(MSVC)
    target_compile_options(project_options INTERFACE
        $<$<CONFIG:Debug>:/W4 /Zi /Od>
        $<$<CONFIG:Release>:/W4 /O2 /DNDEBUG>
    )
else()
    target_compile_options(project_options INTERFACE
        $<$<CONFIG:Debug>:-Wall -Wextra -g -Wimplicit-fallthrough -O0 -Werror=return-type -fsanitize=address>
        $<$<CONFIG:Release>:-Wall -Wextra -O3 -DNDEBUG -Werror=return-type>
    )
    target_link_options(project_options INTERFACE
        $<$<CONFIG:Debug>:-fsanitize=address>
    )
endif()

# add_executable(sound_processor_tests 
#     tests/test_arg_parser.cpp
#     tests/test_wav_file.cpp
#     tests/test_filter.cpp
#     tests/test_filter_producers.cpp

#     ${PARSER_SOURCES}
#     ${WAVFILE_SOURCES}
#     ${FILTER_SOURCES}
#     ${PIPELINE_SOURCES}
#     ${CMDLINEARGS2PIPELINECONVERTER_SOURCES}
#     ${FILTERPRODUCERS_SOURCES}
#     ${APPLICATION_SOURCES}
# )


add_executable(sound_processor 
    src/main.cpp
    ${PARSER_SOURCES}
    ${WAVFILE_SOURCES}
    ${FILTER_SOURCES}
    ${PIPELINE_SOURCES}
    ${CMDLINEARGS2PIPELINECONVERTER_SOURCES}
    ${FILTERPRODUCERS_SOURCES}
    ${APPLICATION_SOURCES}
)

target_link_libraries(sound_processor PRIVATE project_options)
# target_link_libraries(sound_processor_tests PRIVATE project_options Catch2::Catch2WithMain)

target_include_directories(sound_processor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# target_include_directories(sound_processor_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# include(Catch)
# catch_discover_tests(sound_processor_tests)