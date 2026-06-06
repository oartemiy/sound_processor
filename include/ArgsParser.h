#ifndef ARGSPARSER_H_
#define ARGSPARSER_H_

#include "FilterDescriptor.h"
#include <cstddef>
#include <vector>

class ArgsParser
{
public:
    enum class Result
    {
        ok,
        noArgs,
        badArgs
    };

    [[nodiscard]] Result parse(int argc, char* argv[]);

    const char* getInFileName() const { return _inFileName; }
    const char* getOutFileName() const { return _outFileName; }

    const std::vector<FilterDescriptor>& getFilterDescriptors() const
    {
        return _filterDescriptors;
    }

private:
    const char* _inFileName = nullptr;
    const char* _outFileName = nullptr;
    std::vector<FilterDescriptor> _filterDescriptors;

    void addCurrentFilter(std::size_t& paramSize, char**& paramStart,
                          char*& filterName)
    {
        if(filterName)
        {
            _filterDescriptors.emplace_back(filterName, paramStart, paramSize);
            paramSize = 0;
            paramStart = nullptr;
            filterName = nullptr;
        }
    }
};

#endif