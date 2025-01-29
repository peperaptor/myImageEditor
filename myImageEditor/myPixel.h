#ifndef MYPIXEL_H
#define MYPIXEL_H

#include <vector>
#include <string>

struct MyPixel 
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;

    MyPixel();
    void adjustBrightness(const double& coeff);
    void adjustContrast(const int& coeff);

private:
    unsigned char getNewValueBrightness(const unsigned char& initialValue, const double& coeff);
    unsigned char getNewValueContrast(const unsigned char& initialValue, const int& coeff);
};

#endif // MYPIXEL_H