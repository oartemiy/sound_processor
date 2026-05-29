#include "Filter/LowpassFilter.h"
#include "Filter/IFilter.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <new>

IFilter::State LowpassFilter::apply(Waveform& sound)
{
    std::size_t radius = _windowSize / 2;
    std::size_t frames = sound.frameCount();
    if(sound.data.empty())
        return State::emptyWAV;
    try
    {
        std::vector<std::int16_t> newData(sound.data.size());
        for(std::size_t ch = 0; ch < sound.wChannels; ++ch)
        {
            double summa = 0;
            std::int32_t start = (0 - radius) * sound.wChannels + ch;
            std::int32_t end = radius * sound.wChannels + ch;
            for(std::int32_t i = start; i <= end; i += sound.wChannels)
                if(i < 0)
                    summa += sound.data[ch];
                else if(i >= static_cast<std::int32_t>(sound.data.size()))
                    summa += sound.data[(frames - 1) * sound.wChannels + ch];
                else
                    summa += sound.data[i];
            newData[ch] =
                static_cast<std::int16_t>(std::round(summa / _windowSize));

            for(std::size_t i = 1; i < frames; ++i)
            {
                summa -= sound.data[(start > 0) ? start : ch];
                summa +=
                    sound.data[(end + sound.wChannels <
                                static_cast<std::int32_t>(sound.data.size()))
                                   ? end + sound.wChannels
                                   : (frames - 1) * sound.wChannels + ch];

                start += sound.wChannels;
                end += sound.wChannels;

                newData[i * sound.wChannels + ch] =
                    static_cast<std::int16_t>(std::round(summa / _windowSize));
            }
        }

        // for(std::size_t i = 0; i < newData.size(); ++i)
        //     std::cout << newData[i] << ' ';
        // std::cout << std::endl;
        sound.data = std::move(newData);
    }
    catch(std::bad_alloc& err)
    {
        return State::memoryError;
    }
    catch(...)
    {
        return State::unknownError;
    }
    return State::applied;
}