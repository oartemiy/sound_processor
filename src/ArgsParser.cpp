#include "ArgsParser.h"
#include <cctype>
#include <cstddef>
#include <cstring>

// TODO: chang implementation, because of differnet double number form
bool isNumber(const char* str) noexcept
{
    if(!str || !*str)
        return false;
    bool hasPoint = false;
    bool hasDigit = false;
    for(std::size_t i = (str[0] == '-'); str[i] != '\0'; ++i)
    {
        if(std::isdigit(str[i]))
        {
            hasDigit = true;
            continue;
        }
        if(str[i] == '.')
        {
            if(hasPoint)
                return false;
            hasPoint = true;
            continue;
        }
        return false;
    }
    return hasDigit;
}

inline bool isOption(const char* str) noexcept
{
    return str != nullptr && str[0] == '-' && str[1] != '\0' && !isNumber(str);
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
                (isNumber(argv[i]) ||
                 argv[i][0] !=
                     '-'))  // filter param started and it can't be an option
            ++paramSize;

        else
            return Result::badArgs;
    }
    addCurrentFilter(paramSize, paramStart, filterName);
    return Result::ok;
}