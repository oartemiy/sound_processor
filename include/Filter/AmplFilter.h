#ifndef AMPLFILTER_H_
#define AMPLFILTER_H_

#include "IFilter.h"

class AmplFilter: public IFilter
{
public:
    AmplFilter(double ampl): _ampl(ampl) {}

    ~AmplFilter() override {}

    State apply(Waveform& sound) noexcept override;

    const char* getFilterName() noexcept override;

protected:  // to save acess for derived classes
    double _ampl;
};

#endif