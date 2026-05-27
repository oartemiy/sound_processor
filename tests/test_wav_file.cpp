#include "WavFile.h"
#include "Waveform.h"
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdlib>
#include <new>

static std::size_t memoryLimit = 0;
static std::size_t totalAlloc = 0;

void* operator new(std::size_t size)
{
    if(memoryLimit > 0 && totalAlloc + size > memoryLimit)
        throw std::bad_alloc{};
    void* ptr = std::malloc(size);  // NOLINT
    if(!ptr)
        throw std::bad_alloc{};
    totalAlloc += size;
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    std::free(ptr);  // NOLINT
}

// ─────────────────────────────────────────────────────────────
// ✅ ok result
// ─────────────────────────────────────────────────────────────
TEST_CASE("ok test #1")
{
    Waveform file;
    REQUIRE(WavFile::WavError::ok ==
            WavFile::read("/Users/oartemiy/code/sound_processor/tests/samples/"
                          "goodFiles/sample-12s.wav",
                          file));
}

TEST_CASE("ok test #2")
{
    Waveform file;
    auto res1 =
        WavFile::read("/Users/oartemiy/code/sound_processor/tests/samples/"
                      "goodFiles/synth.wav",
                      file);
    auto res2 = WavFile::write("/Users/oartemiy/code/sound_processor/tests/"
                               "samples/outputFiles/result1.wav",
                               file);
    REQUIRE(res1 == WavFile::WavError::ok);
    REQUIRE(res2 == WavFile::WavError::ok);
}

TEST_CASE("good test #3")
{
    Waveform file;
    // NOTE: file with author name in "IART" chunk
    auto res = WavFile::read("/Users/oartemiy/code/sound_processor/tests/"
                             "samples/goodFiles/car-horn.wav",
                             file);
    REQUIRE(res == WavFile::WavError::ok);
}

// ─────────────────────────────────────────────────────────────
// ❌ bad result
// ─────────────────────────────────────────────────────────────
TEST_CASE("bad test #1")
{
    Waveform file;
    auto res = WavFile::read("/Users/oartemiy/code/sound_processor/tests/"
                             "samples/badFiles/badfile1.wav",
                             file);
    REQUIRE(res == WavFile::WavError::invalidRiff);
}

TEST_CASE("bad test #2")
{
    Waveform file;
    auto res = WavFile::read("/Users/oartemiy/code/sound_processor/tests/"
                             "samples/badFiles/badfile2.wav",
                             file);
    REQUIRE(res == WavFile::WavError::missingFmt);
}

TEST_CASE("bad test #3 (memory error)")
{
    memoryLimit = 10 * 1024 * 1024;  // 10MB << 4GB
    Waveform file;
    auto res = WavFile::read("/Users/oartemiy/code/sound_processor/tests/"
                             "samples/badFiles/dataFEFFFF.wav",
                             file);

    REQUIRE(res == WavFile::WavError::memoryError);
    memoryLimit = 0;
}

TEST_CASE("bad test #4")
{
    Waveform file;
    auto res = WavFile::read("/Users/oartemiy/code/sound_processor/tests/"
                             "samples/badFiles/badfile3.wav",
                             file);
    REQUIRE(res == WavFile::WavError::missingData);
}

TEST_CASE("bad test #5")
{
    Waveform file;
    auto res = WavFile::read("/Users/oartemiy/code/sound_processor/tests/"
                             "samples/badFiles/badfile4.wav",
                             file);
    REQUIRE(res == WavFile::WavError::unsupportedFormat);
}
