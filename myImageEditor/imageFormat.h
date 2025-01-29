#ifndef IMAGEFORMAT_H
#define IMAGEFORMAT_H

#include <inttypes.h>
#include <vector>
#include <string>
#include "myPixel.h"
#include <iostream>

class ImageFormat // base class for all image formates (if there will be more than one...)
{
public:

    ImageFormat();
    ImageFormat(const ImageFormat& other);

    virtual void init(const std::wstring& fileName) = 0;
    void init();
    virtual void write() const = 0;

    void adjustBrightness(const double& coeff);
    void adjustContrast(const int& coeff);

    size_t getWidth() const;
    void setWidth(const size_t& width);
    size_t getHeight() const;
    void setHeight(const size_t& height);

    std::vector<MyPixel> pixels;

    std::wstring imageFileName;

private:

    size_t width;
    size_t height;
};

#endif // IMAGEFORMAT_H