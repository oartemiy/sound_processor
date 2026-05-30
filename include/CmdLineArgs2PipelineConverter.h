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
    Pipeline createPipeline(
        const std::vector<ArgsParser::FilterDescriptor>& filterDescriptors);

    void addFilterProducer(const char* filterName, FilterProducer producer);

    FilterProducer getFilterProducer(const char* filterName);

private:
    std::map<std::string_view, FilterProducer> _producers;
};

#endif