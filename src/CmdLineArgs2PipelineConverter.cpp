#include "CmdLineArgs2PipelineConverter.h"
#include "ArgsParser.h"
#include "FilterProducers.h"
#include "Pipeline.h"
#include <stdexcept>

FilterProducer
CmdLineArgs2PipelineConverter::getFilterProducer(const char* filterName)
{
    if(_producers.contains(filterName))
        return _producers[std::string_view{
            filterName}];  // explicit string_view doesn't cause issues
                           // performance because of copy elision
    return nullptr;
}

void CmdLineArgs2PipelineConverter::addFilterProducer(const char* filterName,
                                                      FilterProducer producer)
{
    _producers.emplace(std::string_view{filterName},
                       producer);  // explicit string_view doesn't cause
                                   // performance issues because of copy elision
}

Pipeline CmdLineArgs2PipelineConverter::createPipeline(
    const std::vector<ArgsParser::FilterDescriptor>& filterDescriptors)
{
    Pipeline newPipeline;
    newPipeline.reserveSize(filterDescriptors.size());
    for(const ArgsParser::FilterDescriptor& descriptor: filterDescriptors)
    {
        auto producerPtr = getFilterProducer(descriptor.filterName);
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