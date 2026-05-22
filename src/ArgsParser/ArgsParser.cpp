#include "ArgsParser.h"
#include <cstddef>
#include <cstring>

inline void clearCurFilter(std::size_t& paramSize, char**& paramStart,
                           char*& filterName)
{
    paramSize = 0;
    paramStart = nullptr;
    filterName = nullptr;
}

ArgsParser::Result ArgsParser::parse(int argc, char* argv[])
{
    if(argc == 1)
        return Result::noArgs;
    std::size_t paramSize = 0;
    char** paramStart = nullptr;
    char* filterName = nullptr;
    for(int i = 1; i < argc; ++i)
    {
        if(std::strcmp(argv[i], "-i") == 0)
        {
            if(filterName)
            {
                _filterDescriptors.emplace_back(filterName, paramStart,
                                                paramSize);
                clearCurFilter(paramSize, paramStart, filterName);
            }
            if(_inFileName)
            {
                return Result::badArgs;
            }
            else
            {
                _inFileName = argv[i + 1];
                ++i;
            }
        }
        else if(std::strcmp(argv[i], "-o") == 0)
        {
            if(filterName)
            {
                _filterDescriptors.emplace_back(filterName, paramStart,
                                                paramSize);
                clearCurFilter(paramSize, paramStart, filterName);
            }
            if(_outFileName)
            {
                return Result::badArgs;
            }
            else
            {
                _outFileName = argv[i + 1];
                ++i;
            }
        }
        else if(std::strcmp(argv[i], "-f") == 0)
        {
            if(filterName)
            {
                _filterDescriptors.emplace_back(filterName, paramStart,
                                                paramSize);
                clearCurFilter(paramSize, paramStart, filterName);
            }
            filterName = argv[i + 1];
            paramStart = argv + i + 2;
            paramSize = 0;
            ++i;
        }
        else if(argv[i][0] == '-')  // not permited options
        {
            return Result::badArgs;
        }
        else
        {
            ++paramSize;
        }
    }
    if(filterName)
    {
        _filterDescriptors.emplace_back(filterName, paramStart, paramSize);
        clearCurFilter(paramSize, paramStart, filterName);
    }
    return Result::ok;
}