#include "ciff.h"
#include <limits>

uint intFromBytes(char* bytes, uint size) {
    uint res = 0;
    for (uint i = 0; i < size; ++i) {
        uint val = static_cast<byte>(bytes[i]);
        res = res | val << (size - i - 1) * 4;
    }
    return res;
}

Pixel Ciff::getPixel(uint i) {
    if (i >= pixels.size()) {
        throw out_of_range("Index is out of bound");
    }
    return pixels[i];
}

Ciff Ciff::parse(istream& in, int stage = Ciff::STAGES) {
    uint headerSize;
    uint contentSize;
    uint width;
    uint height;
    string caption;

    //MAGIC
    if (stage >= 1) {
        char magic[5];
        if (in.get(magic, 5) && in.gcount() == 4) {
            if (string{magic} != string("CIFF")) {
                throw domain_error("CIFF should start with 'CIFF'");
            }
        } else {
            throw domain_error("Invalid CIFF format: short magic");
        }
    }

    //Header size
    if (stage >= 2) {
        char headerS[9];
        if (in.get(headerS, 9) && in.gcount() == 8) {
            headerSize = intFromBytes(headerS, 8);
            if (headerSize < Ciff::MIN_HEADER_SIZE) {
                throw domain_error("Invalid headerSize");
            }
        } else {
            throw domain_error("Invalid CIFF format: short headerSize");
        }
    }

    //Content size
    if (stage >= 3) {
        char contentS[9];
        if (in.get(contentS, 9) && in.gcount() == 8) { 
            contentSize = intFromBytes(contentS, 8);
        } else {
            throw domain_error("Invalid CIFF format: short contentSize");
        }
    }

    //Width
    if (stage >= 4) {
        char w[9];
        if (in.get(w, 9) && in.gcount() == 8) { 
            width = intFromBytes(w, 8);
        } else {
            throw domain_error("Invalid CIFF format: short width");
        }
    }

    //Height
    if (stage >= 5) {
        char h[9];
        if (in.get(h, 9) && in.gcount() == 8) { 
            height = intFromBytes(h, 8);
        } else {
            throw domain_error("Invalid CIFF format: short height");
        }

        //Check contentSize
        ull resolution = width * height;
        if (resolution >= numeric_limits<uint>::max() || resolution * 3 != contentSize) {
            throw domain_error("contentSize should equal to width * height * 3");
        }
    }

    return Ciff{headerSize, contentSize, width, height, "", vector<Pixel>(0)};
}