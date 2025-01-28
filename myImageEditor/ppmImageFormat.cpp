#include "ppmImageFormat.h"
#include <string>
#include <cstdio>

void PPMImageFormat::init(const std::string& fileName)
{
    FILE* imageFile = nullptr;
    errno_t err = fopen_s(&imageFile, fileName.c_str(), "rb");
    if (err != 0) // check file
    {
        fclose(imageFile);
        throw std::invalid_argument("Cant open file");
    }
    setImageFileName(fileName);

    char format[3];
    if (fscanf_s(imageFile, "%2s", format, sizeof(format)) != 1 || strcmp(myFileFormat, format)) // check format
    {
        fclose(imageFile);
        throw std::invalid_argument("Wrong file format. PPM P6 needed");
    }

    int c;
    while ((c = fgetc(imageFile)) == '#') // magic thing to skip comments
    {
        while ((c = fgetc(imageFile)) != '\n' && c != EOF);
    }
    ungetc(c, imageFile);

    size_t w, h, maxColorValue;

    if (fscanf_s(imageFile, "%d %d %d", &w, &h, &maxColorValue) == 0 || !checkWidthHeightMaxColorValue(w, h, maxColorValue))
    {
        fclose(imageFile);
        throw std::invalid_argument("Invalid image propertiues");
    }

    setWidth(w);
    setHeight(h);
    
    size_t pixelCount{ getWidth() * getHeight() };

    pixels.resize(pixelCount);

    while ((c = fgetc(imageFile)) != '\n' && c != EOF); // whitespace skip

    for (size_t i = 0; i < pixelCount; ++i)
    {
        unsigned char r, g, b;
        if (fread(&pixels[i].r, sizeof(unsigned char), 1, imageFile) != 1 ||
            fread(&pixels[i].g, sizeof(unsigned char), 1, imageFile) != 1 ||
            fread(&pixels[i].b, sizeof(unsigned char), 1, imageFile) != 1)
        {
            fclose(imageFile);
            throw std::invalid_argument("Cannot read all pixels");
        }
    }

    fclose(imageFile);
}

bool PPMImageFormat::checkWidthHeightMaxColorValue(size_t width, size_t height, size_t maxColorValue)
{
    return (width > 0) && (height > 0) && (maxColorValue > 0 && maxColorValue < 256);
}