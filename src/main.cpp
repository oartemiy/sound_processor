#include "Application.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Application app;
    try
    {
        app.configure();
        app.start(argc, argv);
    }
    catch(std::exception& e)
    {
        std::cerr << "An exception handled: " << e.what() << std::endl;
        return -1;
    }
    catch(...)
    {
        std::cerr << "An unknown exception" << std::endl;
        return -1;
    }
    return 0;
}