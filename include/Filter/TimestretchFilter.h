#ifndef TIMESTRETCHFILTER_H_
#define TIMESTRETCHFILTER_H_

#include "IFilter.h"

class TimestretchFilter: public IFilter
{
public:
    TimestretchFilter(double factor): _factor(factor) {}

    ~TimestretchFilter() override {}

    State apply(Waveform& sound) noexcept override;

    const char* getFilterName() noexcept override;

protected:
    double _factor;
};

#endif