#include "CmdLineArgs2PipelineConverter.h"
#include "FilterProducers.h"
#include "Pipeline.h"
#include <cstring>
#include <stdexcept>
#include <string_view>

FilterProducer
CmdLineArgs2PipelineConverter::getFilterProducer(std::string_view filterName)
{
    if(_producers.contains(filterName))
        return _producers[filterName];
    return nullptr;
}

void CmdLineArgs2PipelineConverter::addFilterProducer(
    std::string_view filterName, FilterProducer producer)
{
    _producers.emplace(filterName,
                       producer);
}

Pipeline CmdLineArgs2PipelineConverter::createPipeline(
    const std::vector<FilterDescriptor>& filterDescriptors)
{
    Pipeline newPipeline;
    newPipeline.reserveSize(filterDescriptors.size());
    for(const FilterDescriptor& descriptor: filterDescriptors)
    {
        std::string filterName = descriptor.filterName;
        if(std::strcmp(descriptor.filterName, "generator") == 0)
            filterName += descriptor.params[0];
        auto producerPtr = getFilterProducer(filterName);
        if(producerPtr)
        {
            auto filterPtr = producerPtr(descriptor);
            if(filterPtr)
                newPipeline.addFilter(std::move(filterPtr));
        }
        else
            throw std::invalid_argument(
                "\"" + std::string(descriptor.filterName) + "\"" +
                " filter does not exists. Check supported filters.");
    }
    return newPipeline;
}