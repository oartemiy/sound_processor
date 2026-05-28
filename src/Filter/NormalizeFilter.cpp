#include "Filter/NormalizeFilter.h"

bool NormalizeFilter::apply(Waveform& sound)
{
    auto currentPeak = sound.getAbsMax();
    if(currentPeak == sound.data.end() || *currentPeak == 0)
        return false;
    auto scale = _peak * Waveform::INT16_SCALE / *currentPeak;
    for(std::size_t i = 0; i < sound.data.size(); ++i)
        sound.setDouble(i, sound.getDouble(i) * scale);
    return true;
}