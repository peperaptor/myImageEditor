#include "myPixel.h"
#include "imageFormat.h"
#include <iostream>

ImageFormat::ImageFormat() :
    width{},
    height{},
    imageFileName{},
    pixels{}
{

}

ImageFormat::ImageFormat(const ImageFormat& other) :
    width(other.getWidth()),
    height(other.getHeight()),
    imageFileName(other.imageFileName),
    pixels(other.pixels)
{

}

void ImageFormat::init()
{
    init(imageFileName);
}

void ImageFormat::adjustBrightness(const double& coeff)
{
    for (auto& pixel : pixels)
    {
        pixel.adjustBrightness(coeff);
    }
}

void ImageFormat::adjustContrast(const int& coeff)
{
    for (auto& pixel : pixels)
    {
        pixel.adjustContrast(coeff);
    }
}

size_t ImageFormat::getWidth() const
{
    return width;
}

void ImageFormat::setWidth(const size_t& width)
{
    this->width = width;
}

size_t ImageFormat::getHeight() const
{
    return height;
}

void ImageFormat::setHeight(const size_t& height)
{
    this->height = height;
}