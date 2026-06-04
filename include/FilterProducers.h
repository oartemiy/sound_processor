#ifndef FILTERPRODUCER_H_
#define FILTERPRODUCER_H_

#include "FilterDescriptor.h"
#include "Filter/Filter.h"
#include <memory>

// factory function
namespace FilterProducers {
std::unique_ptr<Filter>
amplFilterCreator(const FilterDescriptor& descriptor);
std::unique_ptr<Filter>
normalizeFilterCreator(const FilterDescriptor& descriptor);
std::unique_ptr<Filter>
lowpassFilterCreator(const FilterDescriptor& descriptor);
std::unique_ptr<Filter>
silenceFilterCreator(const FilterDescriptor& descriptor);
std::unique_ptr<Filter>
timestretchFilterCreator(const FilterDescriptor& descriptor);
std::unique_ptr<Filter>
sinGenFilterCreator(const FilterDescriptor& descriptor);
std::unique_ptr<Filter>
amGenFilterCreator(const FilterDescriptor& descriptor);
std::unique_ptr<Filter>
fmGenFilterCreator(const FilterDescriptor& descriptor);
}  // namespace FilterProducers

using FilterProducer = std::unique_ptr<Filter> (*) (const FilterDescriptor&);

#endif