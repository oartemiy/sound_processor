#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "CmdLineArgs2PipelineConverter.h"

class Application
{
public:
    void configure();

    // NOTE: May throw exceptions
    void start(int argc, char* argv[]);
protected:
    CmdLineArgs2PipelineConverter _cla2PipelineConverter;
    ArgsParser _parse;
};

#endif