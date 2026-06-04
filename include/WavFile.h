#ifndef WAVREADER_H_
#define WAVREADER_H_

#include "Waveform.h"
#include <cstdint>

namespace __impl { // NOLINT
struct __attribute__((packed)) RiffHeader
{
    std::uint32_t chunkId;
    std::uint32_t chunkSize;
    std::uint32_t waveId;
};

struct __attribute__((packed)) FmtHeader
{
    std::uint32_t chunkId;
    std::uint32_t chunkSize;
    std::uint16_t wFormatTag;
    std::uint16_t wChannels;
    std::uint32_t dwSamplesPerSec;
    std::uint32_t dwAvgBytesPerSec;
    std::uint16_t wBlockAlign;
    std::uint16_t wBitsPerSample;
};

struct __attribute__((packed)) DataHeader
{
    std::uint32_t chunkId;
    std::uint32_t chuckSize;
};
}  // namespace __impl

class WavFile
{
public:
    enum class WavError
    {
        ok,
        fileNotFound,
        invalidRiff,
        missingFmt,
        missingData,
        unsupportedFormat,
        ioError,
        sizeMismatch,
        memoryError,
        unknownError
    };

    // NOTE: path != nullptr
    static WavError read(const char* path, Waveform& outForm);

    // NOTE: path != nullptr
    static WavError write(const char* path, const Waveform& inForm);
};

#endif