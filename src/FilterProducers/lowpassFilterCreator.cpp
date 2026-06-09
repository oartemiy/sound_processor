#include "Filter/LowpassFilter.h"
#include "FilterProducers.h"
#include <cstddef>
#include <cstring>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

std::unique_ptr<Filter>
FilterProducers::lowpassFilterCreator(const FilterDescriptor& descriptor)
{
    if(std::strcmp(descriptor.filterName, "lowpass") != 0)
        throw std::logic_error("Wrong descriptor for lowpass filter producer.");
    if(descriptor.params.size() != 1)
        throw std::logic_error(
            "Wrong params number for lowpass filter producer.");
    try
    {
        std::size_t windowSize = std::stoull(descriptor.params[0]);
        return std::make_unique<LowpassFilter>(windowSize);
    }
    catch(std::invalid_argument& err)
    {
        throw std::logic_error(
            "Error converting arguments for lowpass filter.");
    }
    catch(std::exception& err)
    {
        throw err;
    }
}