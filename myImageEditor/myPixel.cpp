#include <iostream>
#include <fstream>
#include <climits> 
#include <cmath>
#include "myPixel.h"

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
    //r = getNewValueBrightnessNaive(r, coeff);
    //g = getNewValueBrightnessNaive(g, coeff);
    //b = getNewValueBrightnessNaive(b, coeff);

    applyGammaCorrection(r, gamma, coeff);
    applyGammaCorrection(g, gamma, coeff);
    applyGammaCorrection(b, gamma, coeff);
}

unsigned char MyPixel::getNewValueBrightnessNaive(const unsigned char& initialValue, const double& coeff)
{
    return std::min(static_cast<unsigned char>(initialValue * coeff), static_cast<unsigned char>(UCHAR_MAX));
}

void MyPixel::applyGammaCorrection(unsigned char& channel, const double& gamma, const double& coeff)
{
    double normalizedValue = static_cast<double>(channel) / 255.0;
    normalizedValue = std::pow(normalizedValue, 1.0 / gamma) * coeff;
    normalizedValue = std::pow(std::min(normalizedValue, 1.0), gamma);
    channel = static_cast<unsigned char>(std::max(0.0, std::min(255.0, normalizedValue * 255.0)));
}

void MyPixel::adjustContrast(const int& coeff)
{
    if (coeff < -UCHAR_MAX || coeff >= UCHAR_MAX)
    {
        throw std::invalid_argument("Bad coeff");
    }
    //r = getNewValueContrast(r, coeff);
    //g = getNewValueContrast(g, coeff);
    //b = getNewValueContrast(b, coeff);

    double contrast = static_cast<double>(coeff) / 100.0;
    double mean = (static_cast<double>(r) + static_cast<double>(g) + static_cast<double>(b)) / 3.0;
    applyContrast(r, mean, contrast);
    applyContrast(g, mean, contrast);
    applyContrast(b, mean, contrast);
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

void MyPixel::applyContrast(unsigned char& channel, const double& mean, const double& contrast)
{
    double value = static_cast<double>(channel) / 255.0;
    value = 0.5 + contrast * (value - 0.5) + (1.0 - contrast) * (mean / 255.0 - 0.5);
    value = std::max(0.0, std::min(1.0, value));
    channel = static_cast<unsigned char>(value * 255.0);
}