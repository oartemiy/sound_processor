#include "Filter/SinGenFilter.h"
#include <cmath>

bool SinGenFilter::apply(Waveform& sound)
{
    std::size_t newSize =
        std::round(_durationMs / 1000.0 * sound.dwSamplesPerSec);
    sound.data = std::vector<std::int16_t>(newSize);
    constexpr double PI = M_PI;  // NOLINT
    for(std::size_t i = 0; i < newSize; ++i)
    {

        sound.setDouble(i,
                        std::sin(2 * PI * _frequencyHz * i /
                                 static_cast<double>(sound.dwSamplesPerSec)));
    }
    return true;
}