#ifndef PPMIMAGEFORMAT_H
#define PPMIMAGEFORMAT_H
#include "imageFormat.h"
#include <string>

class PPMImageFormat : public ImageFormat
{
public:
    void init(const std::wstring& fileName) override;
    void write() const override;

private:

    const char myFileFormat[3]{ "P6" };

    bool checkWidthHeightMaxColorValue(size_t width, size_t height, size_t maxColorValue) const;
};

#endif // PPMIMAGEFORMAT_H