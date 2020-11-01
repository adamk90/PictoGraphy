#include "ciff.h"

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
    }

    //Content size
    if (stage >= 3) {
        char contentS[9];
        if (in.get(contentS, 9) && in.gcount() == 8) { 
            contentSize = Utils::intFromBytes(contentS, 8);
        } else {
            throw domain_error("Invalid CIFF format: short contentSize");
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
        if (resolution >= numeric_limits<uint>::max() || resolution * 3 != contentSize) {
            throw domain_error("contentSize should equal to width * height * 3");
        }
    }

    //Caption
    if (stage >= 6) {
        //Get the remaining bytes of the header -> Header size - (4 + 8 + 8 + 8 + 8)
        size_t headerContentSize = headerSize - 36;
        char* headerContent = new char[headerContentSize + 1];
        size_t tagsStart = 0;
        if (in.read(headerContent, headerContentSize) && in.gcount() == (streamsize)headerContentSize) {
            size_t i = 0;
            for (; i < headerContentSize; ++i) {
                if (headerContent[i] == '\n') {
                    break;
                }
            }
            if (i == headerContentSize) {
                delete[] headerContent;;
                throw domain_error("Invalid CIFF format: caption should be ended with '\n'");
            }
            caption = string{headerContent, i};
            tagsStart = i + 1;
        } else {
            delete[] headerContent;;
            throw domain_error("Invalid CIFF format: invalid headerSize");
        }

        //Tags
        if (stage >= 7) {
            if (headerContent[headerContentSize - 1] != '\0') {
                delete[] headerContent;;
                throw domain_error("Invalid CIFF format: invalid tags ending");
            }
            size_t currentTagStart = tagsStart;
            for (size_t i = tagsStart; i < headerContentSize; ++i) {
                if (headerContent[i] == '\n') {
                    delete[] headerContent;;
                    throw domain_error("Invalid CIFF format: tags can't be multiline");
                }
                if (headerContent[i] == '\0') {
                    tags.push_back(string{&headerContent[currentTagStart], i - currentTagStart});
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
                    uint red = (uint) pixelContent[0];
                    uint green = (uint) pixelContent[1];
                    uint blue = (uint) pixelContent[2];
                    pixels.push_back(Pixel(red, green, blue));
                } else {
                    delete[] headerContent;;
                    throw domain_error("Invalid CIFF format: contentSize and actual content size does not match");
                }
            }
        }
        delete[] headerContent;
    }

    return Ciff{headerSize, contentSize, width, height, caption, tags, pixels};
}