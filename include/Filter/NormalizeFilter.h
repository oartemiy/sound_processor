#ifndef NORMALIZEFILTER_H_
#define NORMALIZEFILTER_H_

#include "IFilter.h"

class NormalizeFilter: public IFilter
{
public:
    NormalizeFilter(double peak): _peak(peak) {}

    ~NormalizeFilter() override = default;

    State apply(Waveform& sound) noexcept override;

    const char* getFilterName() noexcept override;

protected:
    double _peak;
};

#endif