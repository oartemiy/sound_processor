#ifndef AMGENFILTER_H_
#define AMGENFILTER_H_

#include "AbstractGeneratorFilter.h"
#include "Waveform.h"

class AmGenFilter: public AbstractGeneratorFilter
{
public:
    AmGenFilter(double amplitude, double carrierHz, double modulationHz,
                double depth, double durationMs)
        : _amplitude(amplitude), _carrierHz(carrierHz),
          _modulationHz(modulationHz), _depth(depth), _durationMs(durationMs)
    {
    }

    ~AmGenFilter() override {}

    State apply(Waveform& sound) override;

protected:
    double _amplitude;
    double _carrierHz;
    double _modulationHz;
    double _depth;
    double _durationMs;
};

#endif