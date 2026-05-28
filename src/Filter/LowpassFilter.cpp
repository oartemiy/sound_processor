#include "Filter/LowpassFilter.h"

bool LowpassFilter::apply(Waveform& sound)
{
    // TODO: implement later
    if(sound.wChannels != 1)
        return false;
    std::size_t radius = _windowSize / 2;
    std::size_t frames = sound.frameCount();
    try
    {
        std::vector<std::int16_t> newData(sound.data.size());
        for(std::size_t i = 0; i < frames; ++i)
        {
            std::size_t start = (i >= radius) ? i - radius : 0;
            std::size_t end = (i + radius < frames) ? i + radius : frames - 1;
            std::size_t count = end - start + 1;

            double summa = 0.0;
            // TODO: optimize
            for(std::size_t k = start; k <= end; ++k)
                summa += sound.data[k];
            newData[i] = static_cast<std::int16_t>(std::round(summa / count));
        }
        sound.data = std::move(newData);
    }
    catch(std::bad_alloc& err)
    {
        return false;
    }
    return true;
}