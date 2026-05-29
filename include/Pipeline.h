#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "Filter/Filter.h"
#include <memory>
#include <vector>

class Pipeline
{
public:
    Pipeline(const Pipeline&) = delete;

    Pipeline(Pipeline&&) = default;

    Pipeline& operator=(const Pipeline&) = delete;

    Pipeline& operator=(Pipeline&&) = default;

    

protected:
    std::vector<std::unique_ptr<Filter>> _filters;
};

#endif