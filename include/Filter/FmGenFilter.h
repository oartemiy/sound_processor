#ifndef FMGENFILTER_H_
#define FMGENFILTER_H_

#include "AbstractGeneratorFilter.h"
#include "Waveform.h"

class FmGenFilter: public AbstractGeneratorFilter
{
public:
    FmGenFilter(double amplitude, double carrierHz, double modulationHz,
                double deviationHz, double durationMs)
        : _amplitude(amplitude), _carrierHz(carrierHz),
          _modulationHz(modulationHz), _deviationHz(deviationHz),
          _durationMs(durationMs)
    {
    }

    ~FmGenFilter() override {}

    State apply(Waveform& sound) noexcept override;

    const char* getFilterName() noexcept override;

protected:
    double _amplitude;
    double _carrierHz;
    double _modulationHz;
    double _deviationHz;
    double _durationMs;
};

#endif