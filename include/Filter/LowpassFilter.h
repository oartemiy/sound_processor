#ifndef LOWPASSFILTER_H_
#define LOWPASSFILTER_H_

#include "IFilter.h"

class LowpassFilter: public IFilter
{
public:
    LowpassFilter(std::size_t windowSize): _windowSize(windowSize) {}

    ~LowpassFilter() override {}

    State apply(Waveform& sound) override;

protected:
    std::size_t _windowSize;
};

#endif