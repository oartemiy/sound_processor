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
    char* filterName = nullptr;
    std::size_t paramSize = 0;
    char** paramStart = nullptr;
    for(int i = 1; i < argc; ++i)
    {
        if(std::strcmp(argv[i], "-i") == 0)
        {
            if(filterName)  // end filter reading
            {
                _filterDescriptors.emplace_back(filterName, paramStart,
                                                paramSize);
                clearCurFilter(paramSize, paramStart, filterName);
            }
            if(_inFileName)  // -i option 2 times -> bad
            {
                return Result::badArgs;
            }
            else if(i + 1 != argc && argv[i + 1][0] != '-') // can't be a start of new option
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
            if(filterName)  // end filter reading
            {
                _filterDescriptors.emplace_back(filterName, paramStart,
                                                paramSize);
                clearCurFilter(paramSize, paramStart, filterName);
            }
            if(_outFileName)  // -o option 2 times -> bad
            {
                return Result::badArgs;
            }
            else if(i + 1 != argc && argv[i + 1][0] != '-') // can't be a start of new option
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
            if(filterName)  // end filter reading
            {
                _filterDescriptors.emplace_back(filterName, paramStart,
                                                paramSize);
                clearCurFilter(paramSize, paramStart, filterName);
            }
            if(i + 1 != argc && argv[i + 1][0] != '-') // can't be a start of new option
            {
                filterName = argv[i + 1];
                // TODO: improve stability: may be filters without params
                paramStart = argv + i + 2;
                paramSize = 0;
                ++i;
            }
            else
            {
                return Result::badArgs;
            }
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
    if(filterName) // last filter adding
    {
        _filterDescriptors.emplace_back(filterName, paramStart, paramSize);
        clearCurFilter(paramSize, paramStart, filterName);
    }
    return Result::ok;
}