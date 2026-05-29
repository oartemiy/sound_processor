#ifndef IFILTER_H_
#define IFILTER_H_

#include "Waveform.h"
#include <cstddef>

class IFilter
{
public:
    enum class State
    {
        applied,
        normalizationFailed,
        emptyWAV,
        invalidArgs,
        memoryError,
        unknownError
    };
    
    virtual ~IFilter() {}

    virtual State apply(Waveform& sound) = 0;

    
};


#endif