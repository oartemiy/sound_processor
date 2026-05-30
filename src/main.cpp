#include "Application.h"
#include "iostream"

int main(int argc, char* argv[])
{
    Application app;
    try {
        app.configure();
        app.start(argc, argv);
    }
    catch(std::exception& e)
    {
        std::cerr << "An exception handled: " << e.what();
        return -1;	// a proper named const must appear here
    }
    catch(...)
    {
        std::cerr << "An unknown exception";
        return -2;	// a proper named const must appear here
    }    

    return 0;
}
