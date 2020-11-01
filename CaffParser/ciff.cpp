#include "ciff.h"
#include <iomanip>

Pixel Ciff::getPixel(ull i) {
    if (i >= pixels.size()) {
        throw out_of_range("Index is out of bound");
    }
    return pixels[i];
}

Ciff Ciff::parse(istream& in, int stage = Ciff::STAGES, bool checkSoftLimits = true) {
    ull headerSize;
    ull contentSize;
    ull width;
    ull height;
    string caption;
    vector<string> tags;
    vector<Pixel> pixels;

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
            headerSize = Utils::intFromBytes(headerS, 8);
            if (headerSize < Ciff::MIN_HEADER_SIZE) {
                throw domain_error("Invalid headerSize");
            }
        } else {
            throw domain_error("Invalid CIFF format: short headerSize");
        }
        if (checkSoftLimits && headerSize > Ciff::MAX_CIFF_SIZE) {
            throw domain_error("Ciff file can't exceed 5MB");
        }
    }

    //Content size
    if (stage >= 3) {
        char contentS[9];
        if (in.get(contentS, 9) && in.gcount() == 8) { 
            contentSize = Utils::intFromBytes(contentS, 8);
        } else {
            throw domain_error("Invalid CIFF format: short contentSize");
        }

        ull ciffSize = contentSize + headerSize;
        if (ciffSize < contentSize) {
            throw overflow_error("contentSize + headerSize should fit into 8 bytes");
        }
        if (checkSoftLimits && ciffSize > Ciff::MAX_CIFF_SIZE) {
            throw domain_error("Ciff file can't exceed 5MB");
        }
    }

    //Width
    if (stage >= 4) {
        char w[9];
        if (in.get(w, 9) && in.gcount() == 8) { 
            width = Utils::intFromBytes(w, 8);
        } else {
            throw domain_error("Invalid CIFF format: short width");
        }
    }

    //Height
    if (stage >= 5) {
        char h[9];
        if (in.get(h, 9) && in.gcount() == 8) { 
            height = Utils::intFromBytes(h, 8);
        } else {
            throw domain_error("Invalid CIFF format: short height");
        }

        //Check contentSize
        ull resolution = width * height;
        if (width != 0 && resolution / width != height) {
            throw overflow_error("width * height should fit into 8 bytes");
        }
        ull pixelNumber = resolution * 3;
        if (resolution != 0 && pixelNumber / resolution != 3) {
            throw overflow_error("width * height * 3 should fit into 8 bytes");
        }
        if (pixelNumber != contentSize) {
            throw domain_error("contentSize should equal to width * height * 3");
        }
    }

    //Caption
    if (stage >= 6) {
        //Get the remaining bytes of the header -> Header size - (4 + 8 + 8 + 8 + 8)
        size_t headerContentSize = headerSize - 36;
        unique_ptr<char[]> headerContent{new char[headerContentSize + 1]};
        size_t tagsStart = 0;
        if (in.read(headerContent.get(), headerContentSize) && in.gcount() == (streamsize)headerContentSize) {
            size_t i = 0;
            for (; i < headerContentSize; ++i) {
                if (headerContent.get()[i] == '\n') {
                    break;
                }
            }
            if (i == headerContentSize) {
                throw domain_error("Invalid CIFF format: caption should be ended with '\n'");
            }
            caption = string{headerContent.get(), i};
            tagsStart = i + 1;
        } else {
            throw domain_error("Invalid CIFF format: invalid headerSize");
        }

        //Tags
        if (stage >= 7) {
            if (headerContent.get()[headerContentSize - 1] != '\0') {
                throw domain_error("Invalid CIFF format: invalid tags ending");
            }
            size_t currentTagStart = tagsStart;
            for (size_t i = tagsStart; i < headerContentSize; ++i) {
                if (headerContent.get()[i] == '\n') {
                    throw domain_error("Invalid CIFF format: tags can't be multiline");
                }
                if (headerContent.get()[i] == '\0') {
                    tags.push_back(string{&headerContent.get()[currentTagStart], i - currentTagStart});
                    currentTagStart = i + 1;
                }
            }
        }

        //Pixels
        if (stage >= 8) {
            pixels.reserve(contentSize / 3);
            for (size_t p = 0; p < contentSize / 3; ++p) {
                char pixelContent[3];
                if (in.read(pixelContent, 3) && in.gcount() == 3) {
                    ull red = (ull) pixelContent[0];
                    ull green = (ull) pixelContent[1];
                    ull blue = (ull) pixelContent[2];
                    pixels.push_back(Pixel(red, green, blue));
                } else {
                    throw domain_error("Invalid CIFF format: contentSize and actual content size does not match");
                }
            }
        }
    }
    return Ciff{headerSize, contentSize, width, height, caption, tags, pixels};
}

shared_ptr<byte> Ciff::getBMP(ull& bmpSize) {
    //Determined size is 54 byte + pixels.size() * 3 bytes
    //Note that BMP width and height can only be 4 bytes, but CIFF defines 8 byte long width and height
    //First check for compatibility
    if (width > 4294967295 || height > 4294967295) {
        throw domain_error("Height and width can be max 4 bytes big each");
    }

    //lines should be padded to nearest 4-byte
    int widthPad = 4 - ((width * 3) % 4);

    bmpSize = 54 + ((width * 3 + widthPad) * height);
    byte* bmp = new byte[bmpSize];

    //bmp header
    bmp[0] = 0x42; // B
    bmp[1] = 0x4D; // M
    Utils::fillWithIntToBytes(bmp, bmpSize, 2, 4);
    bmp[6] = bmp[7] = bmp[8] = bmp[9] = 0x00;
    bmp[11] = bmp[12] = bmp[13] = 0x00;
    bmp[10] = 0x36; // No color palette
    bmp[15] = bmp[16] = bmp[17] = 0x00;
    bmp[14] = 0x28;
    Utils::fillWithIntToBytes(bmp, width, 18, 4);
    Utils::fillWithIntToBytes(bmp, height, 22, 4);
    bmp[26] = 0x01; //num of color planes
    bmp[27] = 0x00; //num of color planes
    bmp[28] = 0x18; //24 bits per pixel
    bmp[29] = 0x00; 
    bmp[30] = bmp[31] = bmp[32] = bmp[33] = 0x00; //No compression
    bmp[34] = bmp[35] = bmp[36] = bmp[37] = 0x00; //No compression
    bmp[38] = bmp[39] = bmp[40] = bmp[41] = 0x00;
    bmp[42] = bmp[43] = bmp[44] = bmp[45] = 0x00;
    bmp[46] = bmp[47] = bmp[48] = bmp[49] = 0x00;
    bmp[50] = bmp[51] = bmp[52] = bmp[53] = 0x00;

    //pixels
    ull index = 54;
    ull currByteNum = 0;
    for (const Pixel& p : pixels) {
        bmp[index++] = p.red;
        bmp[index++] = p.green;
        bmp[index++] = p.blue;
        ++currByteNum;
        if (currByteNum % width == 0) {
            for(int i = 0; i < widthPad; ++i) {
                bmp[index++] = 0x00;
            }
        }
    }
    return shared_ptr<byte>(bmp, default_delete<byte[]>{});
}