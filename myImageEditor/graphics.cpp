
#include <windows.h>
#include <CommCtrl.h>
#include <Commdlg.h>
#include "graphics.h"
#include "ppmImageFormat.h"

Graphics::Graphics(ImageFormat& img, ImageFormat& newImage) :
    sourceImage(img),
    currentImage(newImage),
    hInstance(GetModuleHandle(NULL)), 
    hwnd(NULL), 
    hBitmap(NULL),
    hSlider1(NULL),
    hSlider2(NULL),
    slider1Value(slider1Base),
    slider2Value(slider2Base)
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

    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

    hwnd = CreateWindowEx(0, 
                          CLASS_NAME, 
                          L"Image Viewer", 
                          WS_OVERLAPPEDWINDOW | WS_SYSMENU,
                          CW_USEDEFAULT, 
                          CW_USEDEFAULT, 
                          rect.right - rect.left,
                          rect.bottom - rect.top,
                          NULL,
                          NULL, 
                          hInstance, 
                          this);


    if (!hwnd)
    {
        exit(1);
    }

    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    AppendMenu(hFileMenu, MF_STRING, 1, L"Open file");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu(hFileMenu, MF_STRING, 2, L"Save file");
    SetMenu(hwnd, hMenu);

    ShowWindow(hwnd, SW_SHOW);
}

void Graphics::createBitmap(const ImageFormat& image)
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
    if (hBitmap)
    {
        InvalidateRect(hwnd, NULL, TRUE);
        DeleteObject(hBitmap);
    }
    hBitmap = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, bitmapData.data(), &bmi, DIB_RGB_COLORS);
    ReleaseDC(hwnd, hdc);
}

void Graphics::createSlider(HWND& hwnd, size_t positionW, size_t positionH, size_t id, size_t min, size_t max, size_t base)
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

    SendMessage(hwnd, TBM_SETRANGE, TRUE, MAKELONG(min, max));
    SendMessage(hwnd, TBM_SETPOS, TRUE, base);
}


void Graphics::run()
{
    createWindow();
    createSlider(hSlider1, 50, 20, 1, slider1Min, slider1Max, slider1Base);
    createSlider(hSlider2, 50, 55, 2, slider2Min, slider2Max, slider2Base);
    createBitmap(sourceImage);
    InvalidateRect(hwnd, NULL, TRUE);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
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

                RECT rect;
                GetClientRect(hwnd, &rect);
                FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
                
                if (graphics->hBitmap)
                {
                    HDC hdcMem = CreateCompatibleDC(hdc);
                    SelectObject(hdcMem, graphics->hBitmap);

                    BitBlt(hdc, 30, 100, graphics->sourceImage.getWidth(), graphics->sourceImage.getHeight(), hdcMem, 0, 0, SRCCOPY);

                    DeleteDC(hdcMem);
                }
                EndPaint(hwnd, &ps);
                break;
            }
            case WM_HSCROLL:
            {
                HWND slider = reinterpret_cast<HWND>(lParam);
                if (slider == graphics->hSlider1)
                {
                    graphics->slider1Value = SendMessage(slider, TBM_GETPOS, 0, 0);
                    graphics->updateImage(true, false);
                }
                else if (slider == graphics->hSlider2)
                {
                    graphics->slider2Value = SendMessage(slider, TBM_GETPOS, 0, 0);
                    graphics->updateImage(false, true);
                }
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            }
            case WM_COMMAND:
            {
                if (LOWORD(wParam) == 1) 
                {
                    OPENFILENAME ofn;
                    wchar_t fileName[MAX_PATH] = L"";

                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFilter = L"PPM Files\0*.ppm\0All Files\0*.*\0";
                    ofn.lpstrFile = fileName;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

                    if (GetOpenFileName(&ofn))
                    {
                        try
                        {
                            graphics->sourceImage.init(fileName);
                        }
                        catch (const std::invalid_argument& e)
                        {
                            std::cout << ":( " << e.what() << std::endl;
                        }
                    }
                }
                else if (LOWORD(wParam) == 2)
                {
                    try
                    {
                        graphics->currentImage.write();
                    }
                    catch (const std::invalid_argument& e)
                    {
                        std::cout << "oops " << e.what() << std::endl;
                    }
                }

                InvalidateRect(hwnd, NULL, TRUE);
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

void Graphics::updateImage(bool fromSlider1, bool fromSlider2)
{
    currentImage = { sourceImage };
    if (fromSlider1)
    {
        currentImage.adjustBrightness(static_cast<double>(slider1Value / 100.0));
    }
    else if (fromSlider2)
    {
        currentImage.adjustContrast(static_cast<int>(slider2Value));
    }

    createBitmap(currentImage);
}