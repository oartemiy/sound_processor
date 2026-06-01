#include "Filter/SilenceFilter.h"
#include "Filter/IFilter.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

IFilter::State SilenceFilter::apply(Waveform& sound) noexcept
{
    if((std::strcmp(_unit, "ms") != 0 && std::strcmp(_unit, "sec") != 0) ||
       _start < 0 || _end < _start)
        return State::invalidArgs;
    // Work in seconds
    auto scaleFactor = (std::strcmp(_unit, "ms") == 0) ? 0.001 : 1.0;
    auto startSec = _start * scaleFactor;
    auto endSec = _end * scaleFactor;

    // 1 Frame = Channel size * SamplePerSecond
    try
    {
        double startSample = startSec * sound.dwSamplesPerSec;
        double endSample = endSec * sound.dwSamplesPerSec;

        double startFrameDouble = startSample * sound.wChannels;
        double silenceFrameDuractionDouble =
            (endSample - startSample + 0.5) * sound.wChannels;  // round up

        auto startFrame = static_cast<std::size_t>(startFrameDouble);
        auto silenceFramesDuraction =
            static_cast<std::size_t>(silenceFrameDuractionDouble);

        std::size_t newSize = sound.data.size() + silenceFramesDuraction;
        std::vector<std::int16_t> newData;
        newData.reserve(newSize);
        if(startSec >= sound.durationSeconds())
        {
            newData = sound.data;  // copy
            newData.insert(newData.end(), silenceFramesDuraction, 0);
        }
        else
        {
            newData.insert(newData.end(), sound.data.begin(),
                           sound.data.begin() + startFrame);
            newData.insert(newData.end(), silenceFramesDuraction, 0);
            newData.insert(newData.end(), sound.data.begin() + startFrame,
                           sound.data.end());
        }
        sound.data = std::move(newData);
    }
    catch(std::bad_alloc& err)
    {
        return State::memoryError;
    }
    catch(std::exception& err)
    {
        return State::unknownError;
    }
    return State::applied;
}

const char* SilenceFilter::getFilterName() noexcept { return "silence filter"; }