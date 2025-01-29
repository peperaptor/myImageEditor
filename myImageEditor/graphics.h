#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <windows.h>
#include <vector>
#include "imageFormat.h"

class Graphics
{
public:
    Graphics(ImageFormat& image, ImageFormat& newImage);
    ~Graphics();

    void run();

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    ImageFormat& sourceImage;
    ImageFormat& currentImage;

    HINSTANCE hInstance;
    HWND hwnd;
    HWND hSlider1;
    HWND hSlider2;
    HBITMAP hBitmap;

    int slider1Value;
    int slider2Value;

    void createWindow();
    void createSlider(HWND& hwnd, size_t positionW, size_t positionH, size_t id, size_t min, size_t max, size_t base);
    void createBitmap(const ImageFormat& image);
    void updateImage(bool fromSlider1, bool fromSlider2); // lot of things can be done with enum, but im just too lazy

    const int slider1Min = { 0 }, slider1Max{ 200 }, slider1Base{ 100 };
    const int slider2Min = { -255 }, slider2Max{ 255 }, slider2Base{ 0 };
};

#endif // GRAPHIC_H