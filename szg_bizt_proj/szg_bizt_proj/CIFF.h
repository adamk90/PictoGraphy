#pragma once
#include <iostream>
struct Rgb
{
    int r, g, b;
    Rgb();
    Rgb(int c);
    Rgb(int _r, int _g, int _b);
    bool operator != (const Rgb& c) const;
    Rgb& operator *= (const Rgb& rgb);
    Rgb& operator += (const Rgb& rgb);
    //friend int& operator += (int& f, const Rgb rgb);
    void setRGB(int r, int g, int b);
};

class CIFF {
private:
    const char* magic;
    long long headerSize;
    long long contentSize;
    long long width, height; // Image resolution 
    Rgb* pixels; // 1D array of pixels 
    std::string caption;
    std::string tags;

public:

    CIFF();
    CIFF(long long headerSize, long long contentSize, long long width, long long height, const Rgb* pixelArray, std::string caption, std::string tags);
    const Rgb& operator [] (const unsigned int& i) const;
    Rgb& operator [] (const unsigned int& i);
    ~CIFF();

};
