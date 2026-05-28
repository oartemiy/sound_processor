#include "Filter/AmGenFilter.h"
#include "Waveform.h"
#include <cmath>
#include <cstddef>

bool AmGenFilter::apply(Waveform& sound)
{
    std::size_t newSize =
        std::round(_durationMs / 1000.0 * sound.dwSamplesPerSec);
    sound.data = std::vector<std::int16_t>(newSize);
    constexpr double PI = M_PI;  // NOLINT
    for(std::size_t i = 0; i < newSize; ++i) {
        auto envelope = 1.0 + _depth * std::sin(2 * PI * _modulationHz * i /
                                                  sound.dwSamplesPerSec);
        auto carrier =
            std::sin(2 * PI * _carrierHz * i / sound.dwSamplesPerSec);
        sound.setDouble(i, _amplitude * envelope * carrier);
    }
    return true;
}