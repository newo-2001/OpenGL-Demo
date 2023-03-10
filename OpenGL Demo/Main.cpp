#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>

#include "GL/glew.h"
#include "Game.h"

void PrintException(const std::exception& err, int level = 0)
{
    std::cerr << std::string(level * 4, ' ') << err.what() << std::endl;
    
    try
    {
        std::rethrow_if_nested(err);
    }
    catch (std::exception& nested)
    {
        PrintException(nested, level + 1);
    } catch (...) { }
}

int main()
{
    try
    {
        Game::Run();
    }
    catch (std::exception& err)
    {
        PrintException(err);
        return 1;
    }

    return 0;
}
