#ifndef IFILTER_H_
#define IFILTER_H_

#include "Waveform.h"
#include <cstddef>

class IFilter
{
public:
    virtual ~IFilter() {}

    virtual bool apply(Waveform& sound) = 0;
};


#endif