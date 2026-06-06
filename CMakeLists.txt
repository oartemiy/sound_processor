cmake_minimum_required(VERSION 3.24.0)

project(sound_processor VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# order is vital
include(cmake/Sources.cmake)
include(cmake/BuildFlags.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(cmake/Catch2.cmake)
endif()

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

target_include_directories(sound_processor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)