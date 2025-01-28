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
    //ImageFormat(ImageFormat& other);//do something

    virtual void init(const std::string& fileName) = 0;

    size_t getWidth() const;
    void setWidth(const size_t& width);
    size_t getHeight() const;
    void setHeight(const size_t& height);
    std::string getImageFileName() const;
    void setImageFileName(const std::string& fileName);

    friend std::ostream& operator<<(std::ostream& os, const ImageFormat& imageFormat);

    std::vector<MyPixel> pixels;

private:

    size_t width;
    size_t height;
    std::string imageFileName;
};

#endif // IMAGEFORMAT_H