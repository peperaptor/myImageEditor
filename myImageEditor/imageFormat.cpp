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

//ImageFormat::ImageFormat(ImageFormat & other)
//{
//    this->setWidth(other.getWidth());
//    this->setHeight(other.getHeight());
//    this->setImageFileName(other.getImageFileName());
//    this->pixels = {other.pixels};
//}

std::ostream& operator<<(std::ostream& os, const ImageFormat& imageFormat)
{
    os << imageFormat.getWidth() << ' / ' << imageFormat.getHeight() << ' / ' << imageFormat.getImageFileName();
    return os;
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

std::string ImageFormat::getImageFileName() const
{
    return imageFileName;
}

void ImageFormat::setImageFileName(const std::string& fileName)
{
    imageFileName = fileName;
}