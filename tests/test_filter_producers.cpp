#include "ArgsParser.h"
#include "Filter/AmplFilter.h"
#include "Filter/SilenceFilter.h"
#include "FilterProducers.h"
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

std::vector<char*> makeArgv(std::vector<std::string>& args)
{
    std::vector<char*> argv;
    argv.reserve(args.size() + 1);
    for(const auto& arg: args)
        argv.push_back(const_cast<char*>(arg.c_str()));
    argv.push_back(nullptr);  // POSIX-requirement
    return argv;
}

TEST_CASE("nothrow test #1")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {"sound_processor",
                                        "-i",
                                        "input.wav",
                                        "-o",
                                        "output.wav",
                                        "-f",
                                        "ampl",
                                        "0.8",
                                        "-f",
                                        "silence",
                                        "sec",
                                        "0.2",
                                        "0.4"};
    auto argv = makeArgv(vecArgv);
    parse.parse(13, argv.data()); // NOLINT
    const auto& descriptors = parse.getFilterDescriptors();
    REQUIRE(descriptors.size() >= 2);
    CHECK_NOTHROW(FilterProducers::amplFilterCreator(descriptors[0]));
    auto ptr1 = FilterProducers::amplFilterCreator(descriptors[0]);
    CHECK_NOTHROW(FilterProducers::silenceFilterCreator(descriptors[1]));
    auto ptr2 = FilterProducers::silenceFilterCreator(descriptors[1]);
    REQUIRE(ptr1 != nullptr);
    REQUIRE(ptr2 != nullptr);
    REQUIRE(typeid(*ptr1.get()) == typeid(AmplFilter));     // NOLINT
    REQUIRE(typeid(*ptr2.get()) == typeid(SilenceFilter));  // NOLINT
}

TEST_CASE("throw test #1")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {"sound_processor",
                                        "-i",
                                        "input.wav",
                                        "-o",
                                        "output.wav",
                                        "-f",
                                        "ampl",
                                        "0.8",
                                        "-f",
                                        "silence",
                                        "sec",
                                        "0.2",
                                        "0.4"};
    auto argv = makeArgv(vecArgv);
    parse.parse(13, argv.data()); // NOLINT
    const auto& descriptors = parse.getFilterDescriptors();
    REQUIRE(descriptors.size() >= 2);
    CHECK_THROWS(FilterProducers::timestretchFilterCreator(descriptors[0]));
    CHECK_THROWS(FilterProducers::timestretchFilterCreator(descriptors[1]));
    CHECK_THROWS_AS(FilterProducers::timestretchFilterCreator(descriptors[0]),
                    std::logic_error);
    CHECK_THROWS_AS(FilterProducers::timestretchFilterCreator(descriptors[1]),
                    std::logic_error);
}