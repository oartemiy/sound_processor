#include "Filter/SilenceFilter.h"

bool SilenceFilter::apply(Waveform& sound)
{
    // Work in seconds
    auto scaleFactor = (std::strcmp(_unit, "ms") == 0) ? 0.001 : 1.0;
    double startSec = _start * scaleFactor;
    double endSec = _end * scaleFactor;
    if(endSec <= startSec)
        return true;

    double startFrameDouble = startSec * sound.dwSamplesPerSec;
    double endFrameDouble = endSec * sound.dwSamplesPerSec;

    auto startFrame = static_cast<std::size_t>(startFrameDouble);
    auto cntSilenceFrame =
        static_cast<std::size_t>(endFrameDouble - startFrameDouble + 0.5);
    if(cntSilenceFrame == 0)
        return true;
    try
    {
        // 1 Sample = channels * frame, includes channels count
        std::size_t totalFramesOld = sound.frameCount();
        std::size_t startSample = startFrame * sound.wChannels;
        std::size_t silenceSamples = cntSilenceFrame * sound.wChannels;

        std::vector<std::int16_t> newData;
        newData.reserve(sound.data.size() + silenceSamples);

        if(startFrame >= totalFramesOld)
        {
            newData = sound.data;
            newData.insert(newData.end(), silenceSamples, 0);
        }
        else
        {
            newData.insert(newData.end(), sound.data.begin(),
                           sound.data.begin() + startSample);
            newData.insert(newData.end(), silenceSamples, 0);
            newData.insert(newData.end(), sound.data.begin() + startSample,
                           sound.data.end());
        }
        sound.data = std::move(newData);
    }
    catch(std::bad_alloc& err)
    {
        return false;
    }
    return true;
}