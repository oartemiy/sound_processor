#include "FilterProducers.h"
#include <cstring>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

std::unique_ptr<Filter> FilterProducers::sinGenFilterCreator(
    const FilterDescriptor& descriptor)
{
    if(descriptor.params.size() != 3)
        throw std::logic_error(
            "Wrong params number for generator sin filter producer.");
    if(std::strcmp(descriptor.filterName, "generator") != 0 ||
       std::strcmp(descriptor.params[0], "sin") != 0)
        throw std::logic_error(
            "Wrong descriptor for generator sin filter producer.");

    try
    {
        double frequencyHz = std::stod(descriptor.params[1]);
        double durationMs = std::stod(descriptor.params[2]);
        return std::make_unique<SinGenFilter>(frequencyHz, durationMs);
    }
    catch(std::invalid_argument& err)
    {
        throw std::logic_error(
            "Error converting arguments for generator sin filter.");
    }
    catch(std::exception& err)
    {
        throw err;
    }
}