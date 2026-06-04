#include "Filter/TimestretchFilter.h"
#include "FilterProducers.h"
#include <cstring>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

std::unique_ptr<Filter> FilterProducers::timestretchFilterCreator(
    const FilterDescriptor& descriptor)
{
    if(std::strcmp(descriptor.filterName, "timestretch") != 0)
        throw std::logic_error(
            "Wrong descriptor for timestretch filter producer.");
    if(descriptor.params.size() != 1)
        throw std::logic_error(
            "Wrong params number for timestretch filter producer.");
    try
    {
        double factor = std::stod(descriptor.params[0]);
        return std::make_unique<TimestretchFilter>(factor);
    }
    catch(std::invalid_argument& err)
    {
        throw std::logic_error(
            "Error converting arguments for timestretch filter.");
    }
    catch(std::exception& err)
    {
        throw err;
    }
}
