#ifndef FILTERPRODUCER_H_
#define FILTERPRODUCER_H_

#include "ArgsParser.h"
#include "Filter/Filter.h"
#include <memory>

// factory function
namespace FilterProducers {
std::unique_ptr<Filter>
amplFilterCreator(const ArgsParser::FilterDescriptor& descriptor);
std::unique_ptr<Filter>
normalizeFilterCreator(const ArgsParser::FilterDescriptor& descriptor);
std::unique_ptr<Filter>
lowpassFilterCreator(const ArgsParser::FilterDescriptor& descriptor);
std::unique_ptr<Filter>
silenceFilterCreator(const ArgsParser::FilterDescriptor& descriptor);
std::unique_ptr<Filter>
timestretchFilterCreator(const ArgsParser::FilterDescriptor& descriptor);
std::unique_ptr<Filter>
sinGenFilterCreator(const ArgsParser::FilterDescriptor& descriptor);
std::unique_ptr<Filter>
amGenFilterCreator(const ArgsParser::FilterDescriptor& descriptor);
std::unique_ptr<Filter>
fmGenFilterCreator(const ArgsParser::FilterDescriptor& descriptor);
}  // namespace FilterProducers

using FilterProducer = std::unique_ptr<Filter> (*) (const ArgsParser::FilterDescriptor&);

#endif