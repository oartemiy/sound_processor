#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "Filter/Filter.h"
#include "Waveform.h"
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>

class Pipeline
{
public:
    Pipeline() = default;
    
    Pipeline(const Pipeline&) = delete;

    Pipeline(Pipeline&&) = default;

    Pipeline& operator=(const Pipeline&) = delete;

    Pipeline& operator=(Pipeline&&) = default;

    Filter::State apply(Waveform& sound) noexcept;

    [[nodiscard]] std::size_t getFilterNumbers() noexcept
    {
        return _filters.size();
    }

    void reserveSize(std::size_t size) { _filters.reserve(size); }

    const std::unique_ptr<Filter>& addFilter(std::unique_ptr<Filter>&& filter)
    {
        _filters.push_back(std::move(filter));
        return *std::prev(_filters.end());
    }

    decltype(auto) operator[](std::size_t idx) const { return _filters[idx]; }

protected:
    std::vector<std::unique_ptr<Filter>> _filters;
};

#endif