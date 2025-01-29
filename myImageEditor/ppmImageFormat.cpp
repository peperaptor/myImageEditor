#include <string>
#include <cstdio>
#include <climits> 
#include <cstdlib>
#include <codecvt>
#include "ppmImageFormat.h"

void PPMImageFormat::init(const std::wstring& fileName)
{
    FILE* imageFile = nullptr;
    errno_t err = _wfopen_s(&imageFile, fileName.c_str(), L"rb");
    if (err != 0)
    {
        throw std::invalid_argument("Cant open file");
    }
    imageFileName = fileName;


    char format[3];
    if (fscanf_s(imageFile, "%2s", format, sizeof(format)) != 1 || strcmp(myFileFormat, format))
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

    if (fscanf_s(imageFile, "%d %d %d", &w, &h, &maxColorValue) != 3 || 
        !checkWidthHeightMaxColorValue(w, h, maxColorValue))
    {
        fclose(imageFile);
        throw std::invalid_argument("Invalid image properties");
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

bool PPMImageFormat::checkWidthHeightMaxColorValue(size_t width, size_t height, size_t maxColorValue) const
{
    return (width > 0) && 
           (height > 0) && 
           (maxColorValue > 0 && maxColorValue <= static_cast<size_t>(UCHAR_MAX));
}


void PPMImageFormat::write() const
{
    if (_wremove(imageFileName.c_str()) != 0 && errno != ENOENT)
    {
        throw(std::invalid_argument("Cannot remove file"));
    }

    FILE* outFile = nullptr;
    errno_t err = _wfopen_s(&outFile, imageFileName.c_str(), L"wb");
    if (err != 0 || outFile == nullptr)
    {
        throw(std::invalid_argument("Cannot create file"));
    }

    fwprintf(outFile, L"%2s\n%d %d\n%d\n", myFileFormat, getWidth(), getHeight(), 255);

    for (const auto& pixel : pixels)
    {
        if (fwrite(&pixel.r, sizeof(unsigned char), 1, outFile) != 1 ||
            fwrite(&pixel.g, sizeof(unsigned char), 1, outFile) != 1 ||
            fwrite(&pixel.b, sizeof(unsigned char), 1, outFile) != 1)
        {
            fclose(outFile);
            throw(std::invalid_argument("Cannot write pixels"));
        }
    }

    fclose(outFile);
}