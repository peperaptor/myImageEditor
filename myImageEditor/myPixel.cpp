#include "myPixel.h"
#include <iostream>
#include <fstream>
#include <climits> 

MyPixel::MyPixel() :
    r{},
    g{},
    b{}
{

}

void MyPixel::adjustBrightness(const double& coeff)
{
    if (coeff < 0 || coeff > 2)
    {
        throw std::invalid_argument("Bad coeff");
    }
    r = getNewValueBrightness(r, coeff);
    g = getNewValueBrightness(g, coeff);
    b = getNewValueBrightness(b, coeff);
}

unsigned char MyPixel::getNewValueBrightness(const unsigned char& initialValue, const double& coeff)
{
    return std::min(static_cast<unsigned char>(initialValue * coeff), static_cast<unsigned char>(UCHAR_MAX));
}


void MyPixel::adjustContrast(const int& coeff)
{
    r = getNewValueContrast(r, coeff);
    g = getNewValueContrast(g, coeff);
    b = getNewValueContrast(b, coeff);
}


unsigned char MyPixel::getNewValueContrast(const unsigned char& initialValue, const int& coeff)
{
    if (coeff < 0)
    {
        return std::max(static_cast<unsigned char>(initialValue + coeff), static_cast<unsigned char>(0));
    }
    else
    {
        return std::min(static_cast<unsigned char>(initialValue + coeff), static_cast<unsigned char>(UCHAR_MAX));
    }
}