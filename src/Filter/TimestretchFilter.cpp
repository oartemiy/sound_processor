#include "Filter/TimestretchFilter.h"

bool TimestretchFilter::apply(Waveform& sound)
{
    // TODO: implement later
    if(sound.wChannels != 1)
        return false;

    std::size_t oldSize = sound.data.size();
    if(oldSize == 0)
        return true;
    std::size_t newSize = std::round(static_cast<double>(oldSize) * _factor);
    try
    {
        std::vector<std::int16_t> newData;
        newData.reserve(newSize);
        for(std::size_t i = 0; i < newSize; ++i)
        {
            double pos = static_cast<double>(i) / _factor;
            std::size_t l = static_cast<std::size_t>(pos);  // NOLINT
            double frac = pos - l;

            if(l + 1 < oldSize)
            {
                double val =
                    sound.data[l] * (1.0 - frac) + sound.data[l + 1] * frac;
                newData.push_back(val);
            }
            else
            {
                newData.push_back(sound.data[l]);
            }
        }
        sound.data = std::move(newData);
    }
    catch(std::bad_alloc& err)
    {
        return false;
    }
    return true;
}