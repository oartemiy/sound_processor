#include "Filter/SilenceFilter.h"
#include "FilterProducers.h"
#include <cstring>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

std::unique_ptr<Filter>
FilterProducers::silenceFilterCreator(const FilterDescriptor& descriptor)
{
    if(std::strcmp(descriptor.filterName, "silence") != 0)
        throw std::logic_error("Wrong descriptor for silence filter producer.");
    if(descriptor.params.size() != 3)
        throw std::logic_error(
            "Wrong params number for silence filter producer.");
    try
    {
        const char* unit = descriptor.params[0];
        double start = std::stod(descriptor.params[1]);
        double end = std::stod(descriptor.params[2]);
        return std::make_unique<SilenceFilter>(unit, start, end);
    }
    catch(std::invalid_argument& err)
    {
        throw std::logic_error("Error converting arguments for ampl filter.");
    }
    catch(std::exception& err)
    {
        throw err;
    }
}