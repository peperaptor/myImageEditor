
#include <windows.h>
#include <CommCtrl.h>
#include "graphics.h"

Graphics::Graphics(const ImageFormat& img) :
    image(img), 
    hInstance(GetModuleHandle(NULL)), 
    hwnd(NULL), 
    hBitmap(NULL),
    hSlider1(NULL),
    hSlider2(NULL)
{

}

Graphics::~Graphics() 
{
    if (hBitmap)
    {
        DeleteObject(hBitmap);
    }
}

void Graphics::createWindow()
{
    const wchar_t CLASS_NAME[] = L"myWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, 
                          CLASS_NAME, 
                          L"Image Viewer", 
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, 
                          CW_USEDEFAULT, 
                          0, 
                          0,           
                          NULL,
                          NULL, 
                          hInstance, 
                          this);

    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE | WS_MINIMIZEBOX);
    SetWindowPos(hwnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);

    if (!hwnd)
    {
        exit(1);
    }

    ShowWindow(hwnd, SW_SHOW);
}

void Graphics::createBitmap()
{
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = image.getWidth();
    bmi.bmiHeader.biHeight = image.getHeight();
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    std::vector<uint8_t> bitmapData(image.getWidth() * image.getHeight() * 3);
    for (int i = 0; i < image.getWidth() * image.getHeight(); ++i)
    {
        bitmapData[i * 3 + 0] = image.pixels[i].b;
        bitmapData[i * 3 + 1] = image.pixels[i].g;
        bitmapData[i * 3 + 2] = image.pixels[i].r;
    }

    HDC hdc = GetDC(hwnd);
    hBitmap = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, bitmapData.data(), &bmi, DIB_RGB_COLORS);
    ReleaseDC(hwnd, hdc);
}

void Graphics::createSlider(HWND& hwnd, size_t positionW, size_t positionH, size_t id)
{
    hwnd = CreateWindowEx(0,
                          L"msctls_trackbar32",
                          NULL,
                          WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_TOOLTIPS,
                          positionW, 
                          positionH,
                          300,
                          30,
                          this->hwnd, 
                          (HMENU)id,
                          hInstance,
                          NULL);

    SendMessage(hwnd, TBM_SETRANGE, TRUE, MAKELONG(0, 100)); // Set range from 0 to 100
    SendMessage(hwnd, TBM_SETPOS, TRUE, 50);
}


void Graphics::run()
{
    createWindow();
    createSlider(hSlider1, 50, 20, 1);
    createSlider(hSlider2, 50, 55, 2);
    createBitmap();

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Graphics::setImage(const ImageFormat& image)
{
    //
}

LRESULT CALLBACK Graphics::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
    }

    Graphics* graphics = reinterpret_cast<Graphics*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (graphics)
    {
        switch (msg)
        {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HDC hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, graphics->hBitmap);

            BitBlt(hdc, 50, 90, graphics->image.getWidth(), graphics->image.getHeight(), hdcMem, 0, 0, SRCCOPY);

            DeleteDC(hdcMem);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
    else
    {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}