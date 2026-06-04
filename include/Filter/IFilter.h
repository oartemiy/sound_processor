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

    IFilter() = default;
    
    virtual ~IFilter() = default;

    [[nodiscard]] virtual State apply(Waveform& sound) noexcept = 0;

    virtual const char* getFilterName() noexcept = 0;

    // [[nodiscard]] derives for other filters
};


#endif