#include "Filter/AmGenFilter.h"
#include "Filter/IFilter.h"
#include "Waveform.h"
#include <cmath>
#include <cstddef>

IFilter::State AmGenFilter::apply(Waveform& sound) noexcept
{
    if(_amplitude > 1 || _amplitude < 0 || _carrierHz < 0 ||
       _modulationHz < 0 || _depth > 1 || _depth < 0 || _modulationHz < 0)
        return State::invalidArgs;
    std::size_t newSize =
        std::round(_durationMs / 1000.0 * sound.dwSamplesPerSec);
    try
    {
        sound.data = std::vector<std::int16_t>(newSize);
        constexpr double PI = M_PI;  // NOLINT
        for(std::size_t i = 0; i < newSize; ++i)
        {
            auto envelope = 1.0 + _depth * std::sin(2 * PI * _modulationHz * i /
                                                    sound.dwSamplesPerSec);
            auto carrier =
                std::sin(2 * PI * _carrierHz * i / sound.dwSamplesPerSec);
            sound.setDouble(i, _amplitude * envelope * carrier);
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