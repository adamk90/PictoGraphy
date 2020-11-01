#include <iostream>

#include "ciff.h"

using namespace std;

void injectCiffToFile(byte* ciff, uint ciffSize, ifstream& ifile) {
    ofstream ofile;
    ofile.open("test.ciff", ofstream::out | ofstream::trunc);
    ofile.write(reinterpret_cast<char *>(ciff), ciffSize);
    ofile.close();
    ifile.open("test.ciff", ifstream::in);
}

bool testCiffWithBadMagic() {
    byte testCiff[] = {'c', 'I', 'F', 'F' //bad magic
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 4, ifile);
    bool result = false;
    try {
        Ciff::parse(ifile, 1);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("CIFF should start with 'CIFF'") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffWithNoMagic() {
    byte testCiff[] = {};
    ifstream ifile;
    injectCiffToFile(testCiff, 0, ifile);
    bool result = false;
    try {
        Ciff::parse(ifile, 1);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: short magic") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffWithHalfMagic() {
    byte testCiff[] = {'C', 'I'};
    ifstream ifile;
    injectCiffToFile(testCiff, 2, ifile);
    bool result = false;
    try {
        Ciff::parse(ifile, 1);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: short magic") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffWithGoodMagic() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 4, ifile);
    bool result = false;
    try {
        Ciff::parse(ifile, 1);
        result = true;
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodHeaderSize1() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x0, 0x0, 0x0, 0x1, 0xe, 0x2, 0x4, 0x0,  //headerSize=123456
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2);
        if (c.getHeaderSize() == 123456) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodHeaderSize2() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,  //headerSize=4294967295
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2);
        if (c.getHeaderSize() == 4294967295) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodHeaderSize3() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x0, 0x0, 0x0, 0x1, 0xe, 0x2, 0x4, 0x0, //headerSize=123456
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2);
        if (c.getHeaderSize() == 123456) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffBadHeaderSize() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x5 //headerSize=37 (MIN_HEADER_SIZE - 1)
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid headerSize") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffNoHeaderSize() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 4, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: short headerSize") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffHalfHeaderSize() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xf, 0xf, 0xf, 0xf
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 8, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: short headerSize") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffGoodContentSize() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,  //headerSize=4294967295
                       0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,  //contentSize=4294967295
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 20, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3);
        if (c.getContentSize() == 4294967295) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffNoContentSize() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, //headerSize=4294967295
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: short contentSize") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffHalfContentSize() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, //headerSize=4294967295
                       0xf, 0xf, 0xf, 0xf
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 16, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: short contentSize") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffGoodWidth() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,  //headerSize=4294967295
                       0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,  //contentSize=4294967295
                       0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,  //width=4294967295
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 28, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 4);
        if (c.getWidth() == 4294967295) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodHeightCorrectContent() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,  //headerSize=4294967295
                       0x0, 0x0, 0x3, 0x8, 0x4, 0x0, 0x0, 0x0,  //contentSize=3686400 (1200*1024*3)
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xb, 0x0,  //width=1200
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0,  //height=1024
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 36, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 5);
        if (c.getWidth() == 1200 && c.getHeight() == 1024) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodHeightWrongContent1() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,  //headerSize=4294967295
                       0x0, 0x0, 0x3, 0x8, 0x4, 0x0, 0x0, 0x1,  //contentSize=3686401 (1200*1024*3 + 1)
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xb, 0x0,  //width=1200
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0,  //height=1024
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 36, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 5);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("contentSize should equal to width * height * 3") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testEmptyCaptionNoTags() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x6, //headerSize=38
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //contentSize=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //width=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //height=0
                       '\n', //caption
                       '\0', //end of tags
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 38, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 6);
        if (c.getCaption().compare("") == 0) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCaptionWithoutEnd() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xb, //headerSize=43
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //contentSize=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //width=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //height=0
                       'v', 'a', 'l', 'a', 'm', 'i', //caption
                       '\0', //end of tags
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 43, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 6);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: caption should be ended with '\n'") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testGoodCaption() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xc, //headerSize=44
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //contentSize=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //width=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //height=0
                       'v', 'a', 'l', 'a', 'm', 'i', '\n', //caption
                       '\0', //end of tags
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 44, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 6);
        if (c.getCaption().compare("valami") == 0) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testNoTagWithoutEnding() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xb, //headerSize=43
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //contentSize=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //width=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //height=0
                       'v', 'a', 'l', 'a', 'm', 'i', '\n', //caption
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 43, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 7);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: invalid tags ending") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testMultilineTag() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, //hederSize=64
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //contentSize=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //width=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //height=0
                       'v', 'a', 'l', 'a', 'm', 'i', '\n', //caption
                       't', 'a', 'g', '1', '\0',
                       't', 'a', 'g', '2', '\0',
                       't', 'a', 'g', '3', '\0',
                       't', 'a', 'g', '\n', '4', '\0',
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 64, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 7);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: tags can't be multiline") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testGoodTags() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xf, //headerSize=63
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //contentSize=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //width=0
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  //height=0
                       'v', 'a', 'l', 'a', 'm', 'i', '\n', //caption
                       't', 'a', 'g', '1', '\0',
                       't', 'a', 'g', '2', '\0',
                       't', 'a', 'g', '3', '\0',
                       't', 'a', 'g', '4', '\0',
                      };
    ifstream ifile;
    vector<string> goodTags = { "tag1", "tag2", "tag3", "tag4" };
    injectCiffToFile(testCiff, 63, ifile);
    bool result = true;
    try {
        Ciff c = Ciff::parse(ifile, 7);
        auto& tags = c.getTags();
        if (tags.size() == goodTags.size()) {
            for (uint i = 0; i < tags.size(); ++i) {
                if (tags[i].compare(goodTags[i]) != 0) {
                    result = false;
                    break;
                }
            }
        }
    } catch (domain_error& e) {
        result = false;
    }
    ifile.close();
    return result;
}

bool testWrongPixelNumber() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xf, //headerSize=63
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3,  //contentSize=3
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1,  //width=1
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1,  //height=1
                       'v', 'a', 'l', 'a', 'm', 'i', '\n', //caption
                       't', 'a', 'g', '1', '\0',
                       't', 'a', 'g', '2', '\0',
                       't', 'a', 'g', '3', '\0',
                       't', 'a', 'g', '4', '\0',
                       0x1, 0x1,
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 65, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 8);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: contentSize and actual content size does not match") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testPixels() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xf, //headerSize=63
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x9,  //contentSize=9
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3,  //width=3
                       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1,  //height=1
                       'v', 'a', 'l', 'a', 'm', 'i', '\n', //caption
                       't', 'a', 'g', '1', '\0',
                       't', 'a', 'g', '2', '\0',
                       't', 'a', 'g', '3', '\0',
                       't', 'a', 'g', '4', '\0',
                       0x1, 0x2, 0x3,
                       0xff, 0xff, 0xff,
                       0x9, 0xa, 0xb,
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 72, ifile);
    bool result = true;
    vector<Pixel> goodPixels = { {1, 2, 3}, {255, 255, 255}, {9, 10, 11} };
    try {
        Ciff c = Ciff::parse(ifile, 8);
        for (size_t i = 0; i < goodPixels.size(); ++i) {
            if (goodPixels[i] != c.getPixel(i)) {
                result = false;
                break;
            }
        }
    } catch (domain_error& e) {
        result = false;
    } catch (out_of_range& e) {
        result = false;
    }
    ifile.close();
    return result;
}

int main()
{
    if (!testCiffWithBadMagic()) {
        cout << "testCiffWithBadMagic failed" << endl;
    } else {
        cout << "testCiffWithBadMagic succeded" << endl;
    }

    if (!testCiffWithNoMagic()) {
        cout << "testCiffWithNoMagic failed" << endl;
    } else {
        cout << "testCiffWithNoMagic succeded" << endl;
    }
    if (!testCiffWithHalfMagic()) {
        cout << "testCiffWithHalfMagic failed" << endl;
    } else {
        cout << "testCiffWithHalfMagic succeded" << endl;
    }

    if (!testCiffWithGoodMagic()) {
        cout << "testCiffWithGoodMagic failed" << endl;
    } else {
        cout << "testCiffWithGoodMagic succeded" << endl;
    }

    if (!testCiffGoodHeaderSize1()) {
        cout << "testCiffGoodHeaderSize1 failed" << endl;
    } else {
        cout << "testCiffGoodHeaderSize1 succeded" << endl;
    }

    if (!testCiffGoodHeaderSize2()) {
        cout << "testCiffGoodHeaderSize2 failed" << endl;
    } else {
        cout << "testCiffGoodHeaderSize2 succeded" << endl;
    }

    if (!testCiffGoodHeaderSize3()) {
        cout << "testCiffGoodHeaderSize3 failed" << endl;
    } else {
        cout << "testCiffGoodHeaderSize3 succeded" << endl;
    }

    if (!testCiffBadHeaderSize()) {
        cout << "testCiffBadHeaderSize failed" << endl;
    } else {
        cout << "testCiffBadHeaderSize succeded" << endl;
    }

    if (!testCiffNoHeaderSize()) {
        cout << "testCiffNoHeaderSize failed" << endl;
    } else {
        cout << "testCiffNoHeaderSize succeded" << endl;
    }

    if (!testCiffHalfHeaderSize()) {
        cout << "testCiffHalfHeaderSize failed" << endl;
    } else {
        cout << "testCiffHalfHeaderSize succeded" << endl;
    }

    if (!testCiffGoodContentSize()) {
        cout << "testCiffGoodContentSize failed" << endl;
    } else {
        cout << "testCiffGoodContentSize succeded" << endl;
    }

    if (!testCiffNoContentSize()) {
        cout << "testCiffNoContentSize failed" << endl;
    } else {
        cout << "testCiffNoContentSize succeded" << endl;
    }

    if (!testCiffHalfContentSize()) {
        cout << "testCiffHalfContentSize failed" << endl;
    } else {
        cout << "testCiffHalfContentSize succeded" << endl;
    }

    if (!testCiffGoodWidth()) {
        cout << "testCiffGoodWidth failed" << endl;
    } else {
        cout << "testCiffGoodWidth succeded" << endl;
    }

    if (!testCiffGoodHeightCorrectContent()) {
        cout << "testCiffGoodHeightCorrectContent failed" << endl;
    } else {
        cout << "testCiffGoodHeightCorrectContent succeded" << endl;
    }

    if (!testCiffGoodHeightWrongContent1()) {
        cout << "testCiffGoodHeightWrongContent1 failed" << endl;
    } else {
        cout << "testCiffGoodHeightWrongContent1 succeded" << endl;
    }

    if (!testEmptyCaptionNoTags()) {
        cout << "testEmptyCaptionNoTags failed" << endl;
    } else {
        cout << "testEmptyCaptionNoTags succeded" << endl;
    }

    if (!testCaptionWithoutEnd()) {
        cout << "testCaptionWithoutEnd failed" << endl;
    } else {
        cout << "testCaptionWithoutEnd succeded" << endl;
    }

    if (!testGoodCaption()) {
        cout << "testGoodCaption failed" << endl;
    } else {
        cout << "testGoodCaption succeded" << endl;
    }

    if (!testNoTagWithoutEnding()) {
        cout << "testNoTagWithoutEnding failed" << endl;
    } else {
        cout << "testNoTagWithoutEnding succeded" << endl;
    }

    if (!testMultilineTag()) {
        cout << "testMultilineTag failed" << endl;
    } else {
        cout << "testMultilineTag succeded" << endl;
    }

    if (!testGoodTags()) {
        cout << "testGoodTags failed" << endl;
    } else {
        cout << "testGoodTags succeded" << endl;
    }

    if (!testWrongPixelNumber()) {
        cout << "testWrongPixelNumber failed" << endl;
    } else {
        cout << "testWrongPixelNumber succeded" << endl;
    }

    if (!testPixels()) {
        cout << "testPixels failed" << endl;
    } else {
        cout << "testPixels succeded" << endl;
    }

    //TODO: write tests to test CAFF!
}