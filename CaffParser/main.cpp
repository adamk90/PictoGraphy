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

bool testCiffEmptyCaptionNoTags() {
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

bool testCiffCaptionWithoutEnd() {
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

bool testCiffGoodCaption() {
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

bool testCiffNoTagWithoutEnding() {
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

bool testCiffMultilineTag() {
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

bool testCiffGoodTags() {
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

bool testCiffWrongPixelNumber() {
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

bool testCiffPixels() {
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
    vector<pair<function<bool()>, string>> tests{
        {&testCiffWithBadMagic, "testCiffWithBadMagic"},
        {&testCiffWithNoMagic, "testCiffWithNoMagic"},
        {&testCiffWithHalfMagic, "testCiffWithHalfMagic"},
        {&testCiffWithGoodMagic, "testCiffWithGoodMagic"},
        {&testCiffGoodHeaderSize1, "testCiffGoodHeaderSize1"},
        {&testCiffGoodHeaderSize1, "testCiffGoodHeaderSize2"},
        {&testCiffGoodHeaderSize1, "testCiffGoodHeaderSize3"},
        {&testCiffBadHeaderSize, "testCiffBadHeaderSize"},
        {&testCiffNoHeaderSize, "testCiffNoHeaderSize"},
        {&testCiffHalfHeaderSize, "testCiffHalfHeaderSize"},
        {&testCiffGoodContentSize, "testCiffGoodContentSize"},
        {&testCiffNoContentSize, "testCiffNoContentSize"},
        {&testCiffHalfContentSize, "testCiffHalfContentSize"},
        {&testCiffGoodWidth, "testCiffGoodWidth"},
        {&testCiffGoodHeightCorrectContent, "testCiffGoodHeightCorrectContent"},
        {&testCiffGoodHeightWrongContent1, "testCiffGoodHeightWrongContent1"},
        {&testCiffEmptyCaptionNoTags, "testCiffEmptyCaptionNoTags"},
        {&testCiffCaptionWithoutEnd, "testCiffCaptionWithoutEnd"},
        {&testCiffGoodCaption, "testCiffGoodCaption"},
        {&testCiffNoTagWithoutEnding, "testCiffNoTagWithoutEnding"},
        {&testCiffMultilineTag, "testCiffMultilineTag"},
        {&testCiffGoodTags, "testCiffGoodTags"},
        {&testCiffWrongPixelNumber, "testCiffWrongPixelNumber"},
        {&testCiffPixels, "testCiffPixels"},
        //TODO: write tests to test CAFF!
    };

    for (auto& test : tests) {
        if (!test.first()) {
            cout << test.second << " failed" << endl;
        } else {
            cout << test.second << " succeeded" << endl;
        }
    }
}