
#include <iostream>
#include <windows.h>
#include "ppmImageFormat.h"
#include "imageFormat.h"
#include "Graphics.h"

int main()
{
    PPMImageFormat ppmFormat{};
    PPMImageFormat ppmFormatEmpty{};
    ppmFormat.init(L"example.ppm");
    Graphics graphics(ppmFormat, ppmFormatEmpty);
    graphics.run();
    return 0;
}
