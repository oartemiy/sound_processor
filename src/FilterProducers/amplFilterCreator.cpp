#include "Filter/AmplFilter.h"
#include "Filter/Filter.h"
#include "FilterProducers.h"
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

std::unique_ptr<Filter> FilterProducers::amplFilterCreator(
    const FilterDescriptor& descriptor)
{
    if(std::strcmp(descriptor.filterName, "ampl") != 0)
        throw std::logic_error("Wrong descriptor for ampl filter producer.");
    if(descriptor.params.size() != 1)
        throw std::logic_error("Wrong params number for ampl filter producer.");
    try
    {
        double ampl = std::stod(descriptor.params[0]);
        return std::make_unique<AmplFilter>(ampl);
    }
    catch(std::invalid_argument& err)
    {
        throw std::logic_error(
            "Error converting arguments for ampl filter.");
    }
    catch(std::exception& err)
    {
        throw err;
    }
}