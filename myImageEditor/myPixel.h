#ifndef MYPIXEL_H
#define MYPIXEL_H

#include <vector>
#include <string>

class MyPixel 
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;

    MyPixel();
    void adjustBrightness(const double& coeff);
    void adjustContrast(const int& coeff);


private:
    unsigned char getNewValueBrightnessNaive(const unsigned char& initialValue, const double& coeff);
    unsigned char getNewValueContrast(const unsigned char& initialValue, const int& coeff);

    void applyGammaCorrection(unsigned char& channel, const double& gamma, const double& coeff);
    void applyContrast(unsigned char& channel, const double& mean, const double& contrast);

    double gamma = 2.2;

};

#endif // MYPIXEL_H