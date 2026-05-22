#ifndef ARGSPARSER_H_
#define ARGSPARSER_H_

#include <cstddef>
#include <span>
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

    struct FilterDescriptor
    {
        char* filterName = nullptr;
        std::span<char*> params;

        FilterDescriptor(char* filterName, char** paramStart,
                         std::size_t paramSize)
            : filterName(filterName), params(paramStart, paramSize)
        {
        }
    };

    ArgsParser() = default;

    Result parse(int argc, char* argv[]);

    const char* getInFileName() const { return _inFileName; }
    const char* getOutFileName() const { return _outFileName; }

    const std::vector<FilterDescriptor>& getFilters() const
    {
        return _filterDescriptors;
    }

private:
    char* _inFileName = nullptr;
    char* _outFileName = nullptr;
    std::vector<FilterDescriptor> _filterDescriptors;
};

#endif