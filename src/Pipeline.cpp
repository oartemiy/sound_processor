#include "Pipeline.h"
#include "Waveform.h"

Filter::State Pipeline::apply(Waveform& sound) noexcept
{
    auto soundCopy = sound;
    for(const auto& filter: _filters)
    {
        if(auto state = filter->apply(soundCopy);
           state != Filter::State::applied)
        {
            _errorFilter = filter.get();
            return state;
        }
    }
    sound = std::move(soundCopy);
    return Filter::State::applied;
}