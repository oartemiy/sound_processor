#ifndef CMDLINEARGS2PIPELINECONVERTER_H_
#define CMDLINEARGS2PIPELINECONVERTER_H_

#include "ArgsParser.h"
#include "FilterProducers.h"
#include "Pipeline.h"
#include <map>
#include <string_view>
#include <vector>

class CmdLineArgs2PipelineConverter
{
public:
    [[nodiscard]] Pipeline createPipeline(
        const std::vector<FilterDescriptor>& filterDescriptors);

    void addFilterProducer(std::string_view filterName, FilterProducer producer);

    FilterProducer getFilterProducer(std::string_view filterName);

private:
    std::map<std::string_view, FilterProducer> _producers;
};

#endif