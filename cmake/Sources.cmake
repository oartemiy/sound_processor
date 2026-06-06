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