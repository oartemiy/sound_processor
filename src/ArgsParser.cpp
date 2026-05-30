#include "ArgsParser.h"
#include <cctype>
#include <cstddef>
#include <cstring>

inline bool isOption(const char* str) noexcept
{
    return str != nullptr && str[0] == '-' && str[1] != '\0' &&
           (std::isalpha(str[1]) ||
            (str[1] == '-' && str[2] != '\0' && std::isalpha(str[2])));
}

inline char* getNextValue(int argc, char** argv, int currentIndex) noexcept
{
    if(currentIndex + 1 >= argc)
        return nullptr;
    char* candidate = argv[currentIndex + 1];
    if(isOption(candidate))
        return nullptr;
    return candidate;
}

ArgsParser::Result ArgsParser::parse(int argc, char* argv[])
{
    if(argc <= 1)
        return Result::noArgs;

    char* filterName = nullptr;
    std::size_t paramSize = 0;
    char** paramStart = nullptr;

    for(int i = 1; i < argc; ++i)
    {
        if(std::strcmp(argv[i], "-i") == 0)
        {
            addCurrentFilter(paramSize, paramStart, filterName);
            if(_inFileName)  // -i option 2 times -> bad
                return Result::badArgs;
            char* value = getNextValue(argc, argv, i);
            if(!value)
                return Result::badArgs;
            _inFileName = value;
            ++i;
        }
        else if(std::strcmp(argv[i], "-o") == 0)
        {
            addCurrentFilter(paramSize, paramStart, filterName);
            if(_outFileName)  // -o option 2 times -> bad
                return Result::badArgs;
            char* value = getNextValue(argc, argv, i);
            if(!value)
                return Result::badArgs;
            _outFileName = value;
            ++i;
        }
        else if(std::strcmp(argv[i], "-f") == 0)
        {
            addCurrentFilter(paramSize, paramStart, filterName);

            char* value = getNextValue(argc, argv, i);
            if(!value)
                return Result::badArgs;
            filterName = value;
            paramStart = (i + 2 < argc) ? argv + i + 2 : nullptr;
            paramSize = 0;
            ++i;
        }
        else if(isOption(argv[i]))  // not permited options
            return Result::badArgs;
        else if(filterName &&
                !isOption(
                    argv[i]))  // filter param started and it can't be an option
            ++paramSize;

        else
            return Result::badArgs;
    }
    addCurrentFilter(paramSize, paramStart, filterName);
    return Result::ok;
}