#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <iostream>

typedef unsigned int uint;
typedef unsigned char byte;
typedef unsigned long long ull;

using namespace std;

struct Pixel {
    byte red;
    byte green;
    byte blue;

    Pixel(byte r, byte g, byte b): red(r), green(g), blue(b) {}
    Pixel() {}
};

class Ciff {
private:
    uint headerSize;
    uint contentSize;
    uint width;
    uint height;
    string caption;
    vector<Pixel> pixels;

    Ciff(uint hs, uint cs, uint w, uint h, const string& c, const vector<Pixel>& p):
        headerSize(hs), contentSize(cs), width(w), height(h), caption(c), pixels(p) {};

    Ciff() {};

public:
    //Magic + Header + Content + Width + Height + '\n' + '\0'
    static const int MIN_HEADER_SIZE = 4 + 8 + 8 + 8 + 8 + 1 + 1;
    static const int STAGES = 7;

    inline uint getHeaderSize() { return headerSize; }
    inline uint getContentSize() { return contentSize; }
    inline uint getWidth() { return width; }
    inline uint getHeight() { return height; }
    inline string getCaption() { return caption; }

    Pixel getPixel(uint);
    static Ciff parse(istream&, int);
};
