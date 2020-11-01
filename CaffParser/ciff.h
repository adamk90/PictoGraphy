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
    ull headerSize;
    ull contentSize;
    ull width;
    ull height;
    string caption;
    vector<string> tags;
    vector<Pixel> pixels;
    char* headerContent;

    Ciff(ull hs, ull cs, ull w, ull h, const string& c, const vector<string>& t, const vector<Pixel>& p):
        headerSize(hs), contentSize(cs), width(w), height(h), caption(c), tags(t), pixels(p) {};

    Ciff(): headerSize(0), contentSize(0), width(0), height(0), caption(""), tags(vector<string>{}), pixels(vector<Pixel>{}), headerContent(nullptr) {};

public:
    //Magic + Header + Content + Width + Height + '\n' + '\0'
    static const int MIN_HEADER_SIZE = 4 + 8 + 8 + 8 + 8 + 1 + 1;
    static const ull MAX_CIFF_SIZE = 5242880; // 5MB
    static const int STAGES = 8;

    inline ull getHeaderSize() const { return headerSize; }
    inline ull getContentSize() const { return contentSize; }
    inline ull getWidth() const { return width; }
    inline ull getHeight() const { return height; }
    inline string getCaption() const { return caption; }
    inline const vector<string>& getTags() const { return tags; }

    Pixel getPixel(ull);
    static Ciff parse(istream&, int, bool);
    shared_ptr<byte> getBMP(ull&);
};
