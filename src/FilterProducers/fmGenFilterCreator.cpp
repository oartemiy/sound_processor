#include "Filter/FmGenFilter.h"
#include "FilterProducers.h"

std::unique_ptr<Filter>
FilterProducers::fmGenFilterCreator(const FilterDescriptor& descriptor)
{
    if(descriptor.params.size() != 6)
        throw std::logic_error(
            "Wrong params number for generator fm filter producer.");
    if(std::strcmp(descriptor.filterName, "generator") != 0 ||
       std::strcmp(descriptor.params[0], "fm") != 0)
        throw std::logic_error(
            "Wrong descriptor for generator fm filter producer.");

    try
    {
        double amplitude = std::stod(descriptor.params[1]);
        double carrierHz = std::stod(descriptor.params[2]);
        double modulationHz = std::stod(descriptor.params[3]);
        double deviationHz = std::stod(descriptor.params[4]);
        double durationMs = std::stod(descriptor.params[5]);
        return std::make_unique<FmGenFilter>(amplitude, carrierHz, modulationHz,
                                             deviationHz, durationMs);
    }
    catch(std::invalid_argument& err)
    {
        throw std::logic_error(
            "Error converting arguments for generator fm filter.");
    }
    catch(std::exception& err)
    {
        throw err;
    }
}