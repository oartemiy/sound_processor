#include "Filter/TimestretchFilter.h"
#include <cstddef>

bool TimestretchFilter::apply(Waveform& sound)
{
    std::size_t oldSize = sound.data.size();
    if(oldSize == 0)
        return true;
    std::size_t newSize = std::round(static_cast<double>(oldSize) * _factor);
    try
    {
        std::vector<std::int16_t> newData(newSize);
        for(std::size_t ch = 0; ch < sound.wChannels; ++ch)
        {
            for(std::size_t i = 0; i < newSize / sound.wChannels;
                ++i)  // newSize / channels = newFrames
            {
                double pos = static_cast<double>(i) / _factor;
                std::size_t l = static_cast<std::size_t>(pos);  // NOLINT
                double frac = pos - l;

                std::size_t src1 = l * sound.wChannels + ch;
                std::size_t src2 = (l + 1) * sound.wChannels + ch;
                std::size_t dst = i * sound.wChannels + ch;

                if(src2 < oldSize)
                {
                    double val = sound.data[src1] * (1.0 - frac) +
                                 sound.data[src2] * frac;
                    newData[dst] = static_cast<std::int16_t>(std::round(val));
                }
                else
                {
                    newData[dst] = sound.data[src1];
                }
            }
        }
        sound.data = std::move(newData);
    }
    catch(...)
    {
        return false;
    }
    return true;
}