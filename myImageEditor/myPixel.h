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

private:
    void multiply(const double coeff);
};


#endif // MYPIXEL_H