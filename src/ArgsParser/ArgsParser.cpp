#include "ArgsParser.h"
#include <cctype>
#include <cstddef>
#include <cstring>

bool isNumber(const char* str)
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

inline bool isOption(const char* str)
{
    return str && str[0] == '-' && str[1] != '\0' && !isNumber(str);
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
            {
                return Result::badArgs;
            }
            else if(i + 1 != argc &&
                    !isOption(argv[i + 1]))  // can't be a start of new option
            {
                _inFileName = argv[i + 1];
                ++i;
            }
            else
            {
                return Result::badArgs;
            }
        }
        else if(std::strcmp(argv[i], "-o") == 0)
        {
            addCurrentFilter(paramSize, paramStart, filterName);
            if(_outFileName)  // -o option 2 times -> bad
            {
                return Result::badArgs;
            }
            else if(i + 1 != argc &&
                    !isOption(argv[i + 1]))  // can't be a start of new option
            {
                _outFileName = argv[i + 1];
                ++i;
            }
            else
            {
                return Result::badArgs;
            }
        }
        else if(std::strcmp(argv[i], "-f") == 0)
        {
            addCurrentFilter(paramSize, paramStart, filterName);
            if(i + 1 != argc &&
               !isOption(argv[i + 1]))  // can't be a start of new option
            {
                filterName = argv[i + 1];
                // TODO: improve: may be filters without params
                paramStart = argv + i + 2;
                paramSize = 0;
                ++i;
            }
            else
            {
                return Result::badArgs;
            }
        }
        else if(argv[i][0] == '-' &&
                !isNumber(argv[i]))  // not permited options
        {
            return Result::badArgs;
        }
        else
        {
            ++paramSize;
        }
    }
    addCurrentFilter(paramSize, paramStart, filterName);
    return Result::ok;
}