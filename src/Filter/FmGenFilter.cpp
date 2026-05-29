#include "Filter/FmGenFilter.h"
#include "Filter/IFilter.h"
#include "Waveform.h"
#include <cmath>
#include <cstddef>

IFilter::State FmGenFilter::apply(Waveform& sound)
{
    std::size_t newSize =
        std::round(_durationMs / 1000.0 * sound.dwSamplesPerSec);
    try
    {
        sound.data = std::vector<std::int16_t>(newSize);
        constexpr double PI = M_PI;  // NOLINT
        for(std::size_t i = 0; i < newSize; ++i)
        {
            double phase = 2 * PI * _carrierHz * i / sound.dwSamplesPerSec +
                           (_deviationHz / _modulationHz) *
                               std::sin(2 * PI * _modulationHz * i /
                                        sound.dwSamplesPerSec);
            sound.setDouble(i, _amplitude * std::sin(phase));
        }
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