#include "Filter/AmplFilter.h"

bool AmplFilter::apply(Waveform& sound)
{
    if(sound.data.empty())
        return false;
    for(std::size_t i = 0; i < sound.data.size(); ++i)
        sound.setDouble(i, sound.getDouble(i) * _ampl);
    return true;
}