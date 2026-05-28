#ifndef WAVEFORM_H_
#define WAVEFORM_H_

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <vector>

struct Waveform
{
    // private: // after stable release build
    std::uint32_t dwSamplesPerSec = 44100;
    std::uint16_t wBitsPerSample = 16;
    std::uint16_t wChannels = 1;

    std::vector<std::int16_t> data;

    static constexpr double INT16_SCALE = 32767.0;

    [[nodiscard]] std::size_t sampleCount() const noexcept
    {
        return data.size();
    }

    [[nodiscard]] double durationSeconds() const noexcept
    {
        return dwSamplesPerSec > 0 ? static_cast<double>(data.size()) /
                                         (dwSamplesPerSec * wChannels)
                                   : 0.0;
    }

    [[nodiscard]] double durationMilliseconds() const noexcept
    {
        return durationSeconds() * 1000.0;
    }

    [[nodiscard]] std::size_t frameCount() const noexcept
    {
        return wChannels > 0 ? data.size() / wChannels : 0;
    }

    [[nodiscard]] auto getAbsMax() const noexcept
    {
        return std::max_element(data.begin(), data.end(),
                                 [](std::int16_t lhs, std::int16_t rhs)
                                 { return std::abs(lhs) < std::abs(rhs); });
    }

    // ! idx < data.size()
    [[nodiscard]] double getDouble(std::size_t idx) const noexcept
    {
        return static_cast<double>(data[idx]) / INT16_SCALE;
    }

    // ! idx < data.size()
    void setDouble(std::size_t idx, double value) noexcept
    {
        auto clamped = std::clamp(value, -1.0, 1.0);
        data[idx] =
            static_cast<std::int16_t>(std::round(clamped * INT16_SCALE));
    }

    void resizeSeconds(double seconds) noexcept
    {
        auto cnt =
            static_cast<std::size_t>(std::max(0.0, seconds * dwSamplesPerSec));
        data.resize(cnt);
    }

    void resizeMilliseconds(double miliseconds) noexcept
    {
        resizeSeconds(miliseconds / 1000.0);
    }

    void resizeSamples(std::size_t cnt) noexcept { data.resize(cnt); }

    void reserveSamples(std::size_t cnt) noexcept { data.reserve(cnt); }

    void shrinkToFit() noexcept { data.shrink_to_fit(); }
};

#endif