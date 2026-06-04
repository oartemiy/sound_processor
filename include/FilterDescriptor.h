#ifndef FILTERDESCRIPTOR_H_
#define FILTERDESCRIPTOR_H_

#include <span>

struct FilterDescriptor
{
    char* filterName = nullptr;
    std::span<char*> params;

    FilterDescriptor(char* filterName, char** paramStart, std::size_t paramSize)
        : filterName(filterName), params(paramStart, paramSize)
    {
    }
};

#endif