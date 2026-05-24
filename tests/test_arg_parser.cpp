#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <span>
#include <string_view>
#include <vector>
#include <string>

#include "ArgsParser.h"

std::vector<char*> makeArgv(std::vector<std::string>& args)
{
    std::vector<char*> argv;
    argv.reserve(args.size() + 1);
    for(const auto& arg: args)
        argv.push_back(const_cast<char*>(arg.c_str()));
    argv.push_back(nullptr);  // POSIX-requirement
    return argv;
}

// ─────────────────────────────────────────────────────────────
// ✅ ok result
// ─────────────────────────────────────────────────────────────
TEST_CASE("Example test: complex pipeline")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {"sound_processor",
                                        "-i", "input.wav",
                                        "-o", "output.wav",
                                        "-f", "ampl", "0.8",
                                        "-f", "silence", "sec", "0.2", "0.4"};
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(static_cast<int>(vecArgv.size()), argv.data());
    
    REQUIRE(res == ArgsParser::Result::ok);
    REQUIRE(parse.getFilters().size() == 2);
    REQUIRE(parse.getFilters()[0].filterName == vecArgv[6].c_str());
    REQUIRE(parse.getFilters()[0].params[0] == vecArgv[7].c_str());
    REQUIRE(parse.getFilters()[1].filterName == vecArgv[9].c_str());
    REQUIRE(parse.getFilters()[1].params[0] == vecArgv[10].c_str());
    REQUIRE(parse.getFilters()[1].params[1] == vecArgv[11].c_str());
    REQUIRE(parse.getFilters()[1].params[2] == vecArgv[12].c_str());
}

TEST_CASE("No args")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {"sound_processor"};
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(1, argv.data());
    REQUIRE(res == ArgsParser::Result::noArgs);
}

TEST_CASE("-i -o options")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {"sound_processor", "-i", "input.wav",
                                        "-o", "output.wav"};
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(5, argv.data());
    REQUIRE(res == ArgsParser::Result::ok);
    REQUIRE(std::string_view{parse.getInFileName()} == "input.wav");
    REQUIRE(std::string_view{parse.getOutFileName()} == "output.wav");
    REQUIRE(parse.getFilters().empty());
}

TEST_CASE("Filter with numeric parameters")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {
        "sound_processor", "-f", "gain", "2.5", "-10", "0.5", "-o", "out.wav"
    };
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(static_cast<int>(vecArgv.size()), argv.data());
    
    REQUIRE(res == ArgsParser::Result::ok);
    REQUIRE(parse.getFilters().size() == 1);
    REQUIRE(std::string_view{parse.getFilters()[0].filterName} == "gain");
}

TEST_CASE("Negative numbers as filter parameters")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {
        "sound_processor", "-f", "attenuate", "-6.5", "-o", "out.wav"
    };
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(static_cast<int>(vecArgv.size()), argv.data());
    
    REQUIRE(res == ArgsParser::Result::ok);
    // Проверяем, что "-6.5" не был воспринят как опция
    REQUIRE(parse.getFilters().size() == 1);
}

TEST_CASE("Filters before input/output options")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {
        "sound_processor", "-f", "eq", "100", "0.5", "-i", "in.wav", "-o", "out.wav"
    };
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(static_cast<int>(vecArgv.size()), argv.data());
    
    REQUIRE(res == ArgsParser::Result::ok);
    REQUIRE(std::string_view{parse.getInFileName()} == "in.wav");
    REQUIRE(parse.getFilters().size() == 1);
}

// ─────────────────────────────────────────────────────────────
// ❌ Bad args!!!
// ─────────────────────────────────────────────────────────────
TEST_CASE("Duplicate -i option")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {
        "sound_processor", "-i", "a.wav", "-i", "b.wav"
    };
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(static_cast<int>(vecArgv.size()), argv.data());
    REQUIRE(res == ArgsParser::Result::badArgs);
}

TEST_CASE("Duplicate -o option")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {
        "sound_processor", "-o", "a.wav", "-o", "b.wav"
    };
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(static_cast<int>(vecArgv.size()), argv.data());
    REQUIRE(res == ArgsParser::Result::badArgs);
}

TEST_CASE("Unknown option")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {"sound_processor", "--silent"};
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(2, argv.data());
    REQUIRE(res == ArgsParser::Result::badArgs);
}

TEST_CASE("Option without value: -i")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {"sound_processor", "-i"};
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(2, argv.data());
    REQUIRE(res == ArgsParser::Result::badArgs);
}

TEST_CASE("Option without value: -f")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {"sound_processor", "-f"};
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(2, argv.data());
    REQUIRE(res == ArgsParser::Result::badArgs);
}

TEST_CASE("Parameter without active filter")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {"sound_processor", "orphan_param"};
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(2, argv.data());
    REQUIRE(res == ArgsParser::Result::badArgs);
}

TEST_CASE("-f followed by another option instead of name")
{
    ArgsParser parse;
    std::vector<std::string> vecArgv = {"sound_processor", "-f", "-o", "out.wav"};
    auto argv = makeArgv(vecArgv);
    auto res = parse.parse(4, argv.data());
    REQUIRE(res == ArgsParser::Result::badArgs);
}

// ─────────────────────────────────────────────────────────────
// 🧪 param tests
// ─────────────────────────────────────────────────────────────
TEST_CASE("Valid numeric formats accepted as filter params", "[numbers]")
{
    ArgsParser parse;
    
    // Catch2 автоматически запустит тест для каждого значения
    auto validNum = GENERATE(
        as<std::string>{}, 
        "0", "-5", "3.14", "-0.001", ".5", "5.", "100", "-999.99"
    );
    
    std::vector<std::string> cmd = {
        "sound_processor", "-f", "filter", validNum, "-o", "out.wav"
    };
    auto argv = makeArgv(cmd);
    REQUIRE(parse.parse(static_cast<int>(cmd.size()), argv.data()) == ArgsParser::Result::ok);
}

TEST_CASE("Invalid options rejected", "[reject]")
{
    ArgsParser parse;
    
    auto invalid = GENERATE(
        as<std::string>{},
        "--long-opt", "-X", "-abc", "-"
    );
    
    std::vector<std::string> cmd = {
        "sound_processor", "-f", "filter", invalid, "-o", "out.wav"
    };
    auto argv = makeArgv(cmd);
    REQUIRE(parse.parse(static_cast<int>(cmd.size()), argv.data()) == ArgsParser::Result::badArgs);
}

// ─────────────────────────────────────────────────────────────
// 🔄 order testing
// ─────────────────────────────────────────────────────────────
TEST_CASE("Argument order flexibility")
{
    ArgsParser parse;
    
    SECTION("-o before -i") {
        std::vector<std::string> vecArgv = {
            "sound_processor", "-o", "out.wav", "-i", "in.wav"
        };
        auto argv = makeArgv(vecArgv);
        REQUIRE(parse.parse(static_cast<int>(vecArgv.size()), argv.data()) == ArgsParser::Result::ok);
    }
    
    SECTION("Filter between -i and -o") {
        std::vector<std::string> vecArgv = {
            "sound_processor", "-i", "in.wav", "-f", "blur", "2", "-o", "out.wav"
        };
        auto argv = makeArgv(vecArgv);
        REQUIRE(parse.parse(static_cast<int>(vecArgv.size()), argv.data()) == ArgsParser::Result::ok);
    }
}