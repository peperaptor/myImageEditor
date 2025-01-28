#ifndef MYIMAGEFORMAT_H
#define MYIMAGEFORMAT_H
#include "imageFormat.h"

class MyImageFormat : ImageFormat
{
public:

    void init(const std::string& fileName) override;
};

#endif // MYIMAGEFORMAT_H