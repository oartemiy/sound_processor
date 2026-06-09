#include "Filter/AmGenFilter.h"
#include "FilterProducers.h"
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

std::unique_ptr<Filter>
FilterProducers::amGenFilterCreator(const FilterDescriptor& descriptor)
{
    if(descriptor.params.size() != 6)
        throw std::logic_error(
            "Wrong params number for generator am filter producer.");
    if(std::strcmp(descriptor.filterName, "generator") != 0 ||
       std::strcmp(descriptor.params[0], "am") != 0)
        throw std::logic_error(
            "Wrong descriptor for generator am filter producer.");
    try
    {
        double amplitude = std::stod(descriptor.params[1]);
        double carrierHz = std::stod(descriptor.params[2]);
        double modulationHz = std::stod(descriptor.params[3]);
        double depth = std::stod(descriptor.params[4]);
        double durationMs = std::stod(descriptor.params[5]);
        return std::make_unique<AmGenFilter>(amplitude, carrierHz, modulationHz,
                                             depth, durationMs);
    }
    catch(std::invalid_argument& err)
    {
        throw std::logic_error(
            "Error converting arguments for generator am filter.");
    }
    catch(std::exception& err)
    {
        throw err;
    }
}