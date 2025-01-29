#include <thread>
#include <vector>
#include <stdexcept>
#include "imageFormat.h"
#include "myPixel.h"

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


void ImageFormat::adjustBrightnessInRange(std::vector<MyPixel>::iterator start, std::vector<MyPixel>::iterator end, const double& coeff)
{
    for (auto i = start; i != end; ++i)
    {
        i->adjustBrightness(coeff);
    }
}

void ImageFormat::adjustContrastInRange(std::vector<MyPixel>::iterator start, std::vector<MyPixel>::iterator end, const int& coeff)
{
    for (auto i = start; i != end; ++i)
    {
        i->adjustContrast(coeff);
    }
}

void ImageFormat::adjustBrightness(const double& coeff)
{
    size_t numThreads = std::thread::hardware_concurrency();

    if (numThreads == 0)
    {
        numThreads = 2;
    }

    size_t pixelCount = pixels.size();
    size_t blockSize = pixelCount / numThreads;


    std::vector<std::thread> threads; 
    threads.reserve(numThreads);

    for (size_t i = 0; i < numThreads; i++)
    {
        auto start = pixels.begin() + i * blockSize;
        auto end = (i == numThreads - 1) ? pixels.begin() + pixelCount : start + blockSize;
        threads.emplace_back([this, start, end, coeff]() 
                            {
                                this->adjustBrightnessInRange(start, end, coeff);
                            });
    }

    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

void ImageFormat::adjustContrast(const int& coeff)
{
    size_t numThreads = std::thread::hardware_concurrency();

    if (numThreads == 0)
    {
        numThreads = 2;
    }

    size_t pixelCount = pixels.size();
    size_t blockSize = pixelCount / numThreads;


    std::vector<std::thread> threads;
    threads.reserve(numThreads);

    for (size_t i = 0; i < numThreads; i++)
    {
        auto start = pixels.begin() + i * blockSize;
        auto end = (i == numThreads - 1) ? pixels.begin() + pixelCount : start + blockSize;
        threads.emplace_back([this, start, end, coeff]()
                            {
                                this->adjustContrastInRange(start, end, coeff);
                            });
    }

    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
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