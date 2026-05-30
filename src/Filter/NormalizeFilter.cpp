#include "Filter/NormalizeFilter.h"
#include "Filter/IFilter.h"

IFilter::State NormalizeFilter::apply(Waveform& sound) noexcept
{
    if(_peak > 1 || _peak < 0)
        return State::invalidArgs;
    auto currentPeak = sound.getAbsMax();
    if(currentPeak == sound.data.end())
        return State::emptyWAV;
    else if(*currentPeak == 0)
        return State::normalizationFailed;
    auto scale = _peak * Waveform::INT16_SCALE / *currentPeak;
    for(std::size_t i = 0; i < sound.data.size(); ++i)
        sound.setDouble(i, sound.getDouble(i) * scale);
    return State::applied;
}