#ifndef SELENCEFILTER_H_
#define SELENCEFILTER_H_

#include "IFilter.h"

class SilenceFilter: public IFilter
{
public:
    SilenceFilter(const char* unit, double start, double end)
        : _unit(unit), _start(start), _end(end)
    {
    }

    ~SilenceFilter() override {}

    bool apply(Waveform& sound) override;

protected:
    const char* _unit;
    double _start;
    double _end;
};

#endif