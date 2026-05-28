#include "Filter/SilenceFilter.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>
#include <vector>

bool SilenceFilter::apply(Waveform& sound)
{
    // Work in seconds
    auto scaleFactor = (std::strcmp(_unit, "ms") == 0) ? 0.001 : 1.0;
    auto startSec = _start * scaleFactor;
    auto endSec = _end * scaleFactor;
    if(endSec < startSec)
        return false;
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
    catch(...)
    {
        return false;
    }
    return true;
}