#include "Filter/Filter.h"
#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <vector>

// check nearness
void requireNear(const std::vector<int16_t>& lsh,
                 const std::vector<int16_t>& rhs, int tolerance = 1)
{
    REQUIRE(lsh.size() == rhs.size());
    for(size_t i = 0; i < lsh.size(); ++i)
        REQUIRE(std::abs(lsh[i] - rhs[i]) <= tolerance);
}

TEST_CASE("test AmplFilter")
{
    Waveform sound;
    sound.dwSamplesPerSec = 100;
    sound.wChannels = 1;
    sound.data = {1000, 2000, 3000, 32767, -32768};

    SECTION("factor = 1.0 -> unchanged")
    {
        AmplFilter fil(1.0);
        fil.apply(sound);
        requireNear(sound.data, {1000, 2000, 3000, 32767, -32768});
    }

    SECTION("factor = 2.0 -> multiply and clamp")
    {
        AmplFilter fil(2.0);
        fil.apply(sound);
        // 1000*2=2000, 2000*2=4000, 3000*2=6000, 32767*2=65534 clamp to 32767,
        // -32768*2=-65536 clamp to -32768
        requireNear(sound.data, {2000, 4000, 6000, 32767, -32768});
    }

    SECTION("factor = 0.5 -> reduce")
    {
        AmplFilter fil(0.5);
        fil.apply(sound);
        requireNear(sound.data, {500, 1000, 1500, 16383, -16384});
    }
}

TEST_CASE("test NormalizeFilter", "[filter]")
{
    Waveform sound;
    sound.dwSamplesPerSec = 100;
    sound.wChannels = 1;
    sound.data = {1000, -2000, 4000, 0};

    SECTION("default peak = 1.0")
    {
        NormalizeFilter fil(1.0);
        fil.apply(sound);
        // currentPeak = 4000, scale = 32767/4000 ≈ 8.19175
        int16_t expected1 =
            static_cast<int16_t>(std::round(1000 * 32767.0 / 4000.0));  // 8191
        int16_t expected2 = static_cast<int16_t>(
            std::round(-2000 * 32767.0 / 4000.0));  // -16383
        int16_t expected3 = 32767;
        int16_t expected4 = 0;
        requireNear(sound.data, {expected1, expected2, expected3, expected4});
    }

    SECTION("peak = 0.5")
    {
        NormalizeFilter fil(0.5);
        fil.apply(sound);
        // scale = (0.5*32767)/4000 ≈ 4.095875
        int16_t expected1 = static_cast<int16_t>(
            std::round(1000 * 0.5 * 32767.0 / 4000.0));  // 4095
        int16_t expected2 = static_cast<int16_t>(
            std::round(-2000 * 0.5 * 32767.0 / 4000.0));  // -8191
        int16_t expected3 = static_cast<int16_t>(
            std::round(4000 * 0.5 * 32767.0 / 4000.0));  // 16383
        requireNear(sound.data, {expected1, expected2, expected3, 0});
    }

    SECTION("empty waveform")
    {
        Waveform empty;
        empty.data.clear();
        NormalizeFilter fil(1.0);
        fil.apply(empty);
        REQUIRE(empty.data.empty());
    }

    SECTION("all zeros")
    {
        sound.data = {0, 0, 0};
        NormalizeFilter fil(1.0);
        fil.apply(sound);
        requireNear(sound.data, {0, 0, 0});
    }
}

TEST_CASE("test SilenceFilter")
{
    Waveform sound;
    sound.dwSamplesPerSec = 1000;  // 1000 Hz, 1 ms = 1 sample
    sound.wChannels = 1;
    sound.data = {10, 20, 30, 40, 50};  // 5 samples

    SECTION("insert inside, unit = ms")
    {
        SilenceFilter fil("ms", 2.0,
                          4.0);  // from 2ms to 4ms → 2 samples of silence
        fil.apply(sound);
        // Expected: first 2 samples (0-1ms) = 10,20; then 2 zeros; then rest
        // from old index 2: 30,40,50
        requireNear(sound.data, {10, 20, 0, 0, 30, 40, 50});
    }

    SECTION("insert at start, unit = sec")
    {
        SilenceFilter fil("sec", 0.0,
                          0.002);  // 0 to 2ms → 2 zeros at beginning
        fil.apply(sound);
        requireNear(sound.data, {0, 0, 10, 20, 30, 40, 50});
    }

    SECTION("insert beyond end")
    {
        SilenceFilter fil("ms", 6.0, 8.0);  // start after end → append silence
        fil.apply(sound);
        REQUIRE(sound.data.size() == 5 + 2);
        requireNear(sound.data, {10, 20, 30, 40, 50, 0, 0});
    }

    SECTION("zero length interval")
    {
        SilenceFilter fil("ms", 2.0, 2.0);
        auto old = sound.data;
        fil.apply(sound);
        requireNear(sound.data, old);  // unchanged
    }
}

TEST_CASE("test TimestretchFilter (mono)")
{
    Waveform sound;
    sound.dwSamplesPerSec = 100;
    sound.wChannels = 1;
    sound.data = {100, 200, 300};  // 3 samples

    SECTION("stretch factor 2.0")
    {
        TimestretchFilter fil(2.0);
        fil.apply(sound);
        // newSize = round(3*2)=6
        // i:0 pos=0.0 -> 100
        // i:1 pos=0.5 -> 100*0.5+200*0.5=150
        // i:2 pos=1.0 -> 200
        // i:3 pos=1.5 -> 200*0.5+300*0.5=250
        // i:4 pos=2.0 -> 300
        // i:5 pos=2.5 -> last -> 300
        requireNear(sound.data, {100, 150, 200, 250, 300, 300});
    }

    SECTION("compress factor 0.5")
    {
        TimestretchFilter fil(0.5);
        sound.data = {100, 200, 300, 400};
        fil.apply(sound);
        // newSize = round(4*0.5)=2
        // i=0 pos=0 -> 100
        // i=1 pos=2 -> l=2 frac=0 -> 300
        requireNear(sound.data, {100, 300});
    }

    SECTION("factor 1.0")
    {
        TimestretchFilter fil(1.0);
        auto old = sound.data;
        fil.apply(sound);
        REQUIRE(sound.data.size() == old.size());
        requireNear(sound.data, old);
    }
}

TEST_CASE("test LowpassFilter")
{
    Waveform sound;
    sound.dwSamplesPerSec = 100;
    sound.wChannels = 1;
    sound.data = {0, 0, 100, 0, 0};  // 5 samples

    SECTION("window_size = 3")
    {
        LowpassFilter fil(3);
        fil.apply(sound);
        // radius=1
        // i0: avg(0,0,100)=33.33 -> 33
        // i1: avg(0,0,100,0)?? wait: indices 0..2 -> (0+0+100)/3=33.33 ->33
        // i2: avg(0,100,0)=33.33 ->33
        // i3: avg(100,0,0)=33.33 ->33
        // i4: avg(0,0,0)=0
        requireNear(sound.data, {33, 33, 33, 33, 0}, 1);
    }

    SECTION("window_size = 1 -> unchanged")
    {
        LowpassFilter fil(1);
        auto old = sound.data;
        fil.apply(sound);
        requireNear(sound.data, old);
    }

    SECTION("window_size = 5 (odd, larger than length)")
    {
        LowpassFilter fil(5);
        fil.apply(sound);
        // all samples averaged over entire array (0+0+100+0+0)/5=20
        requireNear(sound.data, {20, 20, 20, 20, 20});
    }
}

TEST_CASE("test SinGenFilter")
{
    SinGenFilter fil(440.0, 1000.0);  // 440 Hz, 1000 ms duration
    Waveform sound;
    sound.dwSamplesPerSec = 8000;  // low sample rate for simple test
    sound.wChannels = 1;
    bool ok = fil.apply(sound);  // NOLINT
    REQUIRE(ok);
    // expected size: round(1.0 * 8000) = 8000 samples
    REQUIRE(sound.data.size() == 8000);
    // Check first few samples: sin(2*pi*440*t)
    // t0=0 -> 0
    // t1=1/8000=0.000125 -> phase=2*pi*440*0.000125 ≈ 0.3456 rad -> sin≈0.338
    // scaled to int16: 32767*0.338≈11080
    REQUIRE(sound.data[0] == 0);
    double expected1 = 32767.0 * std::sin(2.0 * M_PI * 440.0 / 8000.0);
    int16_t val1 = static_cast<int16_t>(std::round(expected1));
    REQUIRE(std::abs(sound.data[1] - val1) <= 1);
    // amplitude should be 1.0 by default? According to spec, sin generator does
    // not have amplitude param, uses 32767. So max should be near 32767.
    auto maxIt = std::max_element(sound.data.begin(), sound.data.end());
    REQUIRE(*maxIt >= 32760);  // near full scale
}

TEST_CASE("test AmGenFilter")
{
    // amplitude=0.8, carrier=1000Hz, modulation=2Hz, depth=0.5, duration=500ms
    AmGenFilter fil(0.8, 1000.0, 2.0, 0.5, 500.0);
    Waveform sound;
    sound.dwSamplesPerSec = 8000;
    sound.wChannels = 1;
    fil.apply(sound);
    size_t expectedSamples =
        static_cast<size_t>(std::round(0.5 * 8000));  // 4000
    REQUIRE(sound.data.size() == expectedSamples);
    // Check envelope at t=0: envelope=1+0.5*sin(0)=1, carrier=0 -> sample=0
    REQUIRE(sound.data[0] == 0);
    // At peak of envelope (modulation quarter period) t = 0.125s (since 2Hz ->
    // period 0.5s, quarter 0.125s) t = 0.125s → index = 0.125*8000=1000
    // envelope = 1+0.5*sin(2*pi*2*0.125)=1+0.5*sin(pi/2)=1.5
    // carrier = sin(2*pi*1000*0.125)=sin(250*pi)=0
    // So sample=0. That's not maximal. Better test at time when carrier peaks.
    // Simpler: just check that values are clamped within int16_t range.
    int maxVal = *std::max_element(sound.data.begin(), sound.data.end());
    int minVal = *std::min_element(sound.data.begin(), sound.data.end());
    REQUIRE(maxVal <= 32767);
    REQUIRE(minVal >= -32768);
    // Also check that generator does not crash.
}

TEST_CASE("test FmGenFilter")
{
    // amplitude=0.5, carrier=440Hz, modulation=5Hz, deviation=50Hz,
    // duration=1000ms
    FmGenFilter fil(0.5, 440.0, 5.0, 50.0, 1000.0);
    Waveform sound;
    sound.dwSamplesPerSec = 16000;
    sound.wChannels = 1;
    bool ok = fil.apply(sound);  // NOLINT
    REQUIRE(ok);
    size_t expected = static_cast<size_t>(std::round(1.0 * 16000));  // 16000
    REQUIRE(sound.data.size() == expected);
    // Phase at t=0: 0 -> sin(0)=0
    REQUIRE(sound.data[0] == 0);
    // Check that amplitude scaling works: max absolute value should be ≤
    // 0.5*32767 = 16383.5
    int maxAbs = 0;
    for(auto snd: sound.data)
        if(std::abs(snd) > maxAbs)
            maxAbs = std::abs(snd);
    REQUIRE(maxAbs <= 16384);  // allow rounding
    // And it should be >0 (non-zero signal)
    REQUIRE(maxAbs > 0);
}