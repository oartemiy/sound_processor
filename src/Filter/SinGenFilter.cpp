#include "Filter/SinGenFilter.h"
#include "Filter/IFilter.h"
#include <cmath>

IFilter::State SinGenFilter::apply(Waveform& sound) noexcept
{
    if(_durationMs < 0 || _frequencyHz < 0)
        return State::invalidArgs;
    std::size_t newSize =
        std::round(_durationMs / 1000.0 * sound.dwSamplesPerSec);
    try
    {
        sound.data = std::vector<std::int16_t>(newSize);
        constexpr double PI = M_PI;  // NOLINT
        for(std::size_t i = 0; i < newSize; ++i)
        {

            sound.setDouble(
                i, std::sin(2 * PI * _frequencyHz * i /
                            static_cast<double>(sound.dwSamplesPerSec)));
        }
    }
    catch(std::bad_alloc& err)
    {
        return State::memoryError;
    }
    catch(std::exception& err)
    {
        return State::unknownError;
    }
    return State::applied;
}

const char* SinGenFilter::getFilterName() noexcept { return "sin generator"; }