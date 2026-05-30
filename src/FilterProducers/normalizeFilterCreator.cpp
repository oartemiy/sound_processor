#include "Filter/NormalizeFilter.h"
#include "FilterProducers.h"
#include <cstring>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

std::unique_ptr<Filter> FilterProducers::normalizeFilterCreator(
    const ArgsParser::FilterDescriptor& descriptor)
{
    if(std::strcmp(descriptor.filterName, "normalize") != 0)
        throw std::logic_error(
            "Wrong descriptor for normalize filter producer.");
    if(descriptor.params.size() != 1)
        throw std::logic_error(
            "Wrong params number for normalize filter producer.");
    try
    {
        double peak = std::stod(descriptor.params[0]);
        return std::make_unique<NormalizeFilter>(peak);
    }
    catch(std::invalid_argument& err)
    {
        throw std::logic_error(
            "Error converting arguments for normalize filter.");
    }
    catch(std::exception& err)
    {
        throw err;
    }
}