#include "WavFile.h"
#include "Waveform.h"
#include <cstdint>
#include <cstring>
#include <fstream>

WavFile::WavError WavFile::read(const char* path, Waveform& outForm)
{
    std::ifstream fin(path, std::ios::binary);
    if(!fin.is_open())
        return WavError::fileNotFound;

    RiffHeader riffHdr;
    fin.read(reinterpret_cast<char*>(&riffHdr), sizeof(riffHdr));
    if(!fin || std::memcmp(&riffHdr.chunkId, "RIFF", 4) != 0 ||
       std::memcmp(&riffHdr.waveId, "WAVE", 4) != 0)
        return WavError::invalidRiff;

    bool hasFmt = false;
    bool hasData = false;
    // all chunks has chuckId, chunkSize
    std::uint32_t chunkId;
    std::uint32_t chunkSize;
    while(fin.read(reinterpret_cast<char*>(&chunkId), sizeof(chunkId))
              .read(reinterpret_cast<char*>(&chunkSize), sizeof(chunkSize)))
    {
        if(std::memcmp(&chunkId, "fmt ", 4) == 0)
        {
            if(hasFmt)
                return WavError::invalidRiff;  // 2 fmt chunks
            if(chunkSize != 16)
                return WavError::unsupportedFormat;

            fin.seekg(-8, std::ios::cur);  // return to prev 8 bytes
            fin.clear();                   // clear flags

            FmtHeader fmt;
            if(!fin.read(reinterpret_cast<char*>(&fmt), sizeof(FmtHeader)))
                return WavError::ioError;

            if(fmt.wFormatTag != 1 || fmt.wBitsPerSample != 16)
                return WavError::unsupportedFormat;

            outForm.dwSamplesPerSec = fmt.dwSamplesPerSec;
            outForm.wBitsPerSample = fmt.wBitsPerSample;
            outForm.wChannels = fmt.wChannels;
            hasFmt = true;
        }
        else if(std::memcmp(&chunkId, "data", 4) == 0)
        {
            if(hasData)
                return WavError::invalidRiff;
            if(chunkSize % sizeof(std::int16_t) != 0)
                return WavError::sizeMismatch;
            outForm.data.resize(chunkSize / sizeof(std::int16_t));
            if(!fin.read(reinterpret_cast<char*>(outForm.data.data()),
                         chunkSize))
                return WavError::ioError;
            hasData = true;
        }
        else
        {
            // else just skip chuck
            fin.seekg(chunkSize, std::ios::cur);
        }
        // if chuckSize is even, then align (has a test case)
        if(chunkSize % 2 != 0)
            fin.seekg(1, std::ios::cur);
    }
    if(!hasFmt)
        return WavError::missingFmt;
    if(!hasData)
        return WavError::missingData;
    return WavError::ok;
}

WavFile::WavError WavFile::write(const char* path, const Waveform& inForm)
{
    // If not exist, creates file
    std::ofstream fout(path, std::ios::binary | std::ios::trunc);
    if(!fout.is_open())
        return WavError::fileNotFound;

    auto dataSize =
        static_cast<std::uint32_t>(inForm.data.size() * sizeof(std::int16_t));
    auto fmtPayloadSize = 16u;  // fmt data chunk size
    auto riffSize =
        4 + (8 + fmtPayloadSize) + (8 + dataSize);  // "WAVE" + "fmt" + data

    constexpr static std::uint32_t RIFF = 0x52494646u;
    constexpr static std::uint32_t WAVE = 0x57415645u;
    constexpr static std::uint32_t FMT = 0x666d7420u;
    constexpr static std::uint32_t DATA = 0x64617461u;

    RiffHeader riffHdr = {RIFF, riffSize, WAVE};
    FmtHeader fmtHdr = {FMT,
                        fmtPayloadSize,
                        1,
                        inForm.wChannels,
                        inForm.dwSamplesPerSec,
                        inForm.dwSamplesPerSec * inForm.wChannels * 2,
                        static_cast<std::uint16_t>(inForm.wChannels * 2),
                        16};
    DataHeader dataHdr = {DATA, dataSize};

    fout.write(reinterpret_cast<char*>(&riffHdr), sizeof(RiffHeader));
    fout.write(reinterpret_cast<char*>(&fmtHdr), sizeof(FmtHeader));
    fout.write(reinterpret_cast<char*>(&dataHdr), sizeof(DataHeader));

    fout.write(reinterpret_cast<const char*>(inForm.data.data()), dataSize);

    if(!fout)
        return WavError::ioError;
    return WavError::ok;
}