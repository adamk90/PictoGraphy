#pragma once
#include "utils.h"

struct Pixel {
    byte red;
    byte green;
    byte blue;

    Pixel(byte r, byte g, byte b): red(r), green(g), blue(b) {}
    Pixel() {}

    bool operator==(const Pixel& other) { return red == other.red && green == other.green && blue == other.blue; }
    bool operator!=(const Pixel& other) { return !(*this == other); }
};

class Ciff {
private:
    uint headerSize;
    uint contentSize;
    uint width;
    uint height;
    string caption;
    vector<string> tags;
    vector<Pixel> pixels;
    char* headerContent;

    Ciff(uint hs, uint cs, uint w, uint h, const string& c, const vector<string>& t, const vector<Pixel>& p):
        headerSize(hs), contentSize(cs), width(w), height(h), caption(c), tags(t), pixels(p) {};

    Ciff() {};

public:
    //Magic + Header + Content + Width + Height + '\n' + '\0'
    static const int MIN_HEADER_SIZE = 4 + 8 + 8 + 8 + 8 + 1 + 1;
    static const int STAGES = 8;

    inline uint getHeaderSize() const { return headerSize; }
    inline uint getContentSize() const { return contentSize; }
    inline uint getWidth() const { return width; }
    inline uint getHeight() const { return height; }
    inline string getCaption() const { return caption; }
    inline const vector<string>& getTags() const { return tags; }

    Pixel getPixel(uint);
    static Ciff parse(istream&, int);
};
