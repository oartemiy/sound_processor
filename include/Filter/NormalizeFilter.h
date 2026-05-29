#ifndef NORMALIZEFILTER_H_
#define NORMALIZEFILTER_H_

#include "IFilter.h"

class NormalizeFilter: public IFilter
{
public:
    NormalizeFilter(double peak): _peak(peak) {}

    ~NormalizeFilter() override {}

    State apply(Waveform& sound) override;

protected:
    double _peak;
};

#endif