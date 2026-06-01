#include "Filter/AmplFilter.h"
#include "Filter/IFilter.h"

IFilter::State AmplFilter::apply(Waveform& sound) noexcept
{
    if(_ampl < 0)
        return State::invalidArgs;
    if(sound.data.empty())
        return State::emptyWAV;
    for(std::size_t i = 0; i < sound.data.size(); ++i)
        sound.setDouble(i, sound.getDouble(i) * _ampl);
    return State::applied;
}

const char* AmplFilter::getFilterName() noexcept { return "ampl filter"; }