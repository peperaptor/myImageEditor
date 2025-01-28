#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <windows.h>
#include <vector>
#include "imageFormat.h"

class Graphics
{
public:
    Graphics(const ImageFormat& image);
    ~Graphics();

    void run();

    void setImage(const ImageFormat& image);

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    const ImageFormat& image;
    HINSTANCE hInstance;
    HWND hwnd;
    HWND hSlider1;
    HWND hSlider2;
    HWND g_hEdit1;
    HWND g_hEdit2;
    HBITMAP hBitmap;

    int sliderValue = 0;

    void createWindow();
    void createSlider(HWND& hwnd, size_t positionW, size_t positionH, size_t id);
    void createBitmap();
};

#endif // GRAPHIC_H