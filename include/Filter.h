#ifndef IFILTER_H_
#define IFILTER_H_
#include "Waveform.h"

class Filter
{
public:
    virtual ~Filter() {}
    virtual bool apply(Waveform& sound) = 0;
};

#endif