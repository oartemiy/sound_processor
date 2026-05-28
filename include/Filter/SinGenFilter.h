#ifndef SINGENFILTER_H_
#define SINGENFILTER_H_

#include "AbstractGeneratorFilter.h"

class SinGenFilter: public AbstractGeneratorFilter
{
public:
    SinGenFilter(double frequencyHz, double durationMs)
        : _frequencyHz(frequencyHz), _durationMs(durationMs)
    {
    }

    ~SinGenFilter() override {}

    bool apply(Waveform& sound) override;

protected:
    double _frequencyHz;
    double _durationMs;
};

#endif