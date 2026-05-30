#include "Application.h"
#include "ArgsParser.h"
#include "Filter/Filter.h"
#include "FilterProducers.h"
#include "WavFile.h"
#include "Waveform.h"
#include <iostream>
#include <stdexcept>
#include <string>

void Application::configure()
{
    // !NOTE: USE ONLY STRING LITERALS!!!
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
        // TODO: implement later...
        std::cout << "help..." << std::endl;
        return;
    }
    Waveform sound;
    auto pathIn = _parse.getInFileName();
    if(pathIn)
    {
        auto readRes = WavFile::read(pathIn, sound);
        if(readRes == WavFile::WavError::fileNotFound)
            throw std::logic_error(std::string{"File: "} + pathIn +
                                   " does not found.");
        else if(readRes == WavFile::WavError::invalidRiff)
            throw std::logic_error(std::string{"File: "} + pathIn +
                                   " has invalid RIFF format.");
        else if(readRes == WavFile::WavError::missingData)
            throw std::logic_error(std::string{"File: "} + pathIn +
                                   " miss DATA chunk.");
        else if(readRes == WavFile::WavError::missingFmt)
            throw std::logic_error(std::string{"File: "} + pathIn +
                                   " miss FMT  chunk");
        else if(readRes == WavFile::WavError::unsupportedFormat)
            throw std::logic_error(std::string{"File: "} + pathIn +
                                   " has unsupported format.");
        else if(readRes == WavFile::WavError::sizeMismatch)
            throw std::logic_error(std::string{"File: "} + pathIn +
                                   " has size mismatch");
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
    // TODO: throw more invalid args info
    if(pipelineRes == Filter::State::invalidArgs)
        throw std::logic_error("Invalid arguments.");
    else if(pipelineRes == Filter::State::emptyWAV)
        throw std::logic_error(std::string{"File: "} + pathIn + " is empty.");
    else if(pipelineRes == Filter::State::memoryError)
        throw std::logic_error("Memory error occurs while filters appling.");
    else if(pipelineRes == Filter::State::normalizationFailed)
        throw std::logic_error("Silence can not be normalized.");
    else if(pipelineRes == Filter::State::unknownError)
        throw std::logic_error("Unknown error occurs while filters appling.");
    else if(pipelineRes == Filter::State::applied)
        std::cout << "All filters ere applied." << std::endl;

    auto pathOut = _parse.getOutFileName();
    if(!pathOut)
    {
        if(!pathIn)
            throw std::logic_error("No file names paramets.");
        pathOut = pathIn;
    }
    auto writeRes = WavFile::write(pathOut, sound);
    // TODO: This error newer occurs
    if(writeRes == WavFile::WavError::fileNotFound)
        throw std::logic_error(std::string{"File: "} + pathOut +
                               " does not found.");
    else if(writeRes == WavFile::WavError::ioError)
        throw std::logic_error("Binary io error occures while file writing.");
    else if(writeRes == WavFile::WavError::ok)
        std::cout << "All changes save to file: " + std::string{pathOut}
                  << std::endl;
}