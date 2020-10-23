#include "CIFF.h"
#include <iostream>


    Rgb::Rgb() : r(0), g(0), b(0) {}
    Rgb::Rgb(int c) : r(c), g(c), b(c) {}
    Rgb::Rgb(int _r, int _g, int _b) : r(_r), g(_g), b(_b) {}
    bool Rgb::operator != (const Rgb& c) const
    {
        return c.r != r || c.g != g || c.b != b;
    }
    Rgb& Rgb::operator *= (const Rgb& rgb)
    {
        r *= rgb.r, g *= rgb.g, b *= rgb.b;
        return *this;
    }
    Rgb& Rgb::operator += (const Rgb& rgb)
    {
        r += rgb.r, g += rgb.g, b += rgb.b;
        return *this;
    }
    /*
    int& Rgb::operator += (int& f, const Rgb rgb)
    {
        f += (rgb.r + rgb.g + rgb.b) / 3;
        return f;
    }
    */
    void Rgb::setRGB(int r, int g, int b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }


    CIFF::CIFF() : magic(NULL), headerSize(0), contentSize(0), width(0), height(0), pixels(nullptr) { /* empty image */ }
    CIFF::CIFF(long long headerSize, long long contentSize, long long width, long long height, const Rgb* pixelArray, std::string caption, std::string tags)
    {
        this->magic = {"CIFF"};
        this->headerSize = headerSize;
        this->contentSize = contentSize;
        this->width = width;
        this->height = height;
        pixels = new Rgb[width * height];
        for (long long i = 0; i < width * height; ++i) {
            pixels[i] = pixelArray[i];
        }
        this->caption = caption;
        this->tags = tags;

    }
    const Rgb& CIFF::operator [] (const unsigned int& i) const
    {
        return pixels[i];
    }
    Rgb& CIFF::operator [] (const unsigned int& i)
    {
        return pixels[i];
    }
    CIFF::~CIFF()
    {
        if (pixels != NULL) delete[] pixels;
    }