#include "ArgsParser/ArgsParser.h"
#include <iostream>

int main(int argc, char* argv[])
{
    ArgsParser parser;
    auto res = parser.parse(argc, argv);
    if(res == ArgsParser::Result::ok)
    {
        std::cout << parser.getInFileName() << ' ' << parser.getOutFileName()
                  << std::endl;
        for(const auto& filter: parser.getFilters())
        {
            std::cout << "=========\n";
            std::cout << filter.filterName << '\n';
            for(auto i: filter.params)
            {
                std::cout << i << std::endl;
            }
        }
    }
    else if(res == ArgsParser::Result::badArgs)
    {
        std::cout << "Bad\n";
    }
    else
    {
        std::cout << "NO args\n";
    }
}