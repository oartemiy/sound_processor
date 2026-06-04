#include "Application.h"
#include "ArgsParser.h"
#include "Filter/AmplFilter.h"
#include "Filter/Filter.h"
#include "FilterProducers.h"
#include "WavFile.h"
#include "Waveform.h"
#include <iostream>
#include <stdexcept>
#include <string>

const char* const HELP_INFO = R"(Usage:
  sound_processor [-i input_file] [-o output_file] [-f filter_name filter_params...] ...

IO FILES OPTIONS
  -i <file>                             read .wav sound info from <file>
  -o <file>                             write sound_processor result sound to <file>
  
FILTERS OPTIONS
  -f <filter_name> <params>             apply filter <filter_name> with <params> to current sound

FILTERS DESCRIPTION

  ampl <factor>                         Multiplies each sample of the signal by the specified factor.


Parameter | Type    | Allowed values | Description
----------|---------|----------------|--------------------------------------
factor    | real    | factor >= 0    | amplitude scaling coefficient


  normalize <peak>                      Scales the signal so that the maximum modulo count is equal to a set fraction of the limit value int16_t.


Parameter | Type    | Allowed values    | Description
----------|---------|-------------------|---------------------------------------------------------
peak      | real    | 0 <= peak <= 1    | target peak as a fraction of 32767, optional, default value is 1


  silence <unit> <start> <end>          Inserts a section of silence (zero counts) into the signal. In this case, the counts after the insertion point are shifted to the right.


Parameter | Type    | Allowed values       | Description
----------|---------|----------------------|---------------------------------------------------
unit      | string  | sec or ms            | time unit for start and end
start     | real    | start >= 0           | start time of silence from the beginning of the signal
end       | real    | end >= start         | end time of silence (inclusive)


  timestretch <factor>                  Changes the duration of the signal by a set number of times. The values of the new signal are obtained by linear interpolation of neighboring samples of the original one.


Parameter | Type    | Allowed values | Description
----------|---------|----------------|-----------------------------
factor    | real    | factor > 0     | duration scaling coefficient


  lowpass <window_size>                 Smoothes the signal with an average over a fixed-size window.


Parameter    | Type    | Allowed values          | Description
-------------|---------|-------------------------|---------------------------------------
window_size  | integer | odd, window_size >= 1   | averaging window size in samples

GENERATORS DESCRIPTION (generator)

  sin <frequency_hz> <duration_ms>      Generates a sinusoid of a given frequency and duration.


Parameter     | Type    | Allowed values      | Description
--------------|---------|---------------------|------------------------------------
frequency_hz  | real    | frequency_hz >= 0   | sine wave frequency, Hz
duration_ms   | real    | duration_ms >= 0    | signal duration, ms


  am <amplitude> <carrier_hz> <modulation_hz> <depth> <duration_ms>
    The signal is a sinusoid (carrier), the amplitude of which varies over time according to the law of another sinusoid. As a result, the volume of the signal periodically increases and decreases.


Parameter      | Type    | Allowed values            | Description
---------------|---------|---------------------------|------------------------------------------------
amplitude      | real    | 0 <= amplitude <= 1       | overall signal amplitude
carrier_hz     | real    | carrier_hz >= 0           | carrier frequency
modulation_hz  | real    | modulation_hz >= 0        | modulating sine wave frequency
depth          | real    | 0 <= depth <= 1           | modulation depth
duration_ms    | real    | duration_ms >= 0          | signal duration, ms


  fm <amplitude> <carrier_hz> <modulation_hz> <deviation_hz> <duration_ms>
    The signal is a sinusoid (carrier), the instantaneous frequency of which is not constant, but deviates from the central one according to the law of another sinusoid (modulating). The maximum frequency deviation from the central frequency is set by the <deviation_hz> parameter.


Parameter      | Type    | Allowed values            | Description
---------------|---------|---------------------------|---------------------------------------------------
amplitude      | real    | 0 <= amplitude <= 1       | signal amplitude
carrier_hz     | real    | carrier_hz >= 0           | carrier center frequency
modulation_hz  | real    | modulation_hz > 0         | modulating sine wave frequency
deviation_hz   | real    | deviation_hz >= 0         | maximum frequency deviation from center
duration_ms    | real    | duration_ms >= 0          | signal duration, ms

)";

void Application::configure()
{
    // !NOTE: USE ONLY STRING LITERALS OR CONSTINIT VARS (static memory)!!!
    _cla2PipelineConverter.addFilterProducer(
        "ampl", FilterProducers::amplFilterCreator);
    _cla2PipelineConverter.addFilterProducer(
        "normalize", FilterProducers::normalizeFilterCreator);
    _cla2PipelineConverter.addFilterProducer(
        "silence", FilterProducers::silenceFilterCreator);
    _cla2PipelineConverter.addFilterProducer(
        "timestretch", FilterProducers::timestretchFilterCreator);
    _cla2PipelineConverter.addFilterProducer(
        "lowpass", FilterProducers::lowpassFilterCreator);
    _cla2PipelineConverter.addFilterProducer(
        "generatorsin", FilterProducers::sinGenFilterCreator);
    _cla2PipelineConverter.addFilterProducer(
        "generatoram", FilterProducers::amGenFilterCreator);
    _cla2PipelineConverter.addFilterProducer(
        "generatorfm", FilterProducers::fmGenFilterCreator);
}

void Application::start(int argc, char* argv[])
{
    auto parseRes = _parse.parse(argc, argv);
    if(parseRes == ArgsParser::Result::badArgs)
        throw std::logic_error("Command line arguments are bad.");
    else if(parseRes == ArgsParser::Result::noArgs)
    {
        std::cout << HELP_INFO << std::endl;
        return;
    }
    Waveform sound;
    auto pathIn = _parse.getInFileName();
    if(pathIn)
    {
        auto readRes = WavFile::read(pathIn, sound);
        if(readRes == WavFile::WavError::fileNotFound)
            throw std::logic_error(std::string{"File: \""} + pathIn +
                                   "\" does not found.");
        else if(readRes == WavFile::WavError::invalidRiff)
            throw std::logic_error(std::string{"File: \""} + pathIn +
                                   "\" has invalid RIFF format.");
        else if(readRes == WavFile::WavError::missingData)
            throw std::logic_error(std::string{"File: \""} + pathIn +
                                   "\" miss DATA chunk.");
        else if(readRes == WavFile::WavError::missingFmt)
            throw std::logic_error(std::string{"File: \""} + pathIn +
                                   "\" miss FMT  chunk");
        else if(readRes == WavFile::WavError::unsupportedFormat)
            throw std::logic_error(std::string{"File: \""} + pathIn +
                                   "\" has unsupported format.");
        else if(readRes == WavFile::WavError::sizeMismatch)
            throw std::logic_error(std::string{"File: \""} + pathIn +
                                   "\" has size mismatch");
        else if(readRes == WavFile::WavError::memoryError)
            throw std::logic_error("Memory error occurs while file reading.");
        else if(readRes == WavFile::WavError::ioError)
            throw std::logic_error(
                "Binary io error occures while file reading.");
        else if(readRes == WavFile::WavError::unknownError)
            throw std::logic_error("Unknown error occurs while file reading.");
    }

    auto pipeline =
        _cla2PipelineConverter.createPipeline(_parse.getFilterDescriptors());

    auto pipelineRes = pipeline.apply(sound);
    if(pipelineRes == Filter::State::invalidArgs)
        throw std::logic_error("Invalid arguments for " +
                               std::string{pipeline.getErrorFilterName()});
    else if(pipelineRes == Filter::State::emptyWAV)
        throw std::logic_error(std::string{"File: \""} + pathIn +
                               "\" is empty.");
    else if(pipelineRes == Filter::State::memoryError)
        throw std::logic_error("Memory error occurs while filters appling.");
    else if(pipelineRes == Filter::State::normalizationFailed)
        throw std::logic_error("Silence can not be normalized.");
    else if(pipelineRes == Filter::State::unknownError)
        throw std::logic_error("Unknown error occurs while filters appling.");
    else if(pipelineRes == Filter::State::applied)
        std::cout << "All filters were applied." << std::endl;

    auto pathOut = _parse.getOutFileName();
    if(!pathOut)
    {
        if(!pathIn)
            throw std::logic_error("No file names paramets.");
        pathOut = pathIn;
    }
    auto writeRes = WavFile::write(pathOut, sound);
    if(writeRes == WavFile::WavError::fileNotFound)
        throw std::logic_error(std::string{"File: \""} + pathOut +
                               "\" Can not be opened or created.");
    else if(writeRes == WavFile::WavError::ioError)
        throw std::logic_error("Binary io error occures while file writing.");
    else if(writeRes == WavFile::WavError::ok)
        std::cout << "All changes saved to file: \"" + std::string{pathOut}
                  << "\"" << std::endl;
}