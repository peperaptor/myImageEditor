
#include <iostream>
#include <windows.h>
#include "ppmImageFormat.h"
#include "imageFormat.h"
#include "Graphics.h"

int main()
{
    PPMImageFormat ppmFormat{};
    try
    {
        ppmFormat.init("input.ppm");
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << "oops " << e.what() << std::endl;
    }
    Graphics graphics(ppmFormat);
    graphics.run();
    return 0;
}
