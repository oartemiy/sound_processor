#ifndef CMDLINEARGS2PIPELINECONVERTER_H_
#define CMDLINEARGS2PIPELINECONVERTER_H_

#include "Pipeline.h"
#include <map>
#include <string_view>
#include <vector>
#include "ArgsParser.h"

class CmdLineArgs2PipelineConverter
{
public:
    Pipeline createPipeline(
        const std::vector<ArgsParser::FilterDescriptor>& filterDescriptors);

private:

};


#endif