#include <iostream>

#include "ciff.h"
#include "caff.h"

using namespace std;

void injectCiffToFile(byte* ciff, ull ciffSize, ifstream& ifile) {
    ofstream ofile;
    ofile.open("test.ciff", ofstream::out | ofstream::trunc | ofstream::binary);
    ofile.write(reinterpret_cast<char *>(ciff), ciffSize);
    ofile.close();
    ifile.open("test.ciff", ifstream::in);
}

void outputBmpToFile(shared_ptr<byte> bmp, ull bmpSize) {
    ofstream ofile;
    ofile.open("test.bmp", ofstream::out | ofstream::trunc | ofstream::binary);
    ofile.write(reinterpret_cast<const char *>(bmp.get()), bmpSize);
    ofile.close();
}

bool testCiffWithBadMagic() {
    byte testCiff[] = {'c', 'I', 'F', 'F' //bad magic
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 4, ifile);
    bool result = false;
    try {
        Ciff::parse(ifile, 1, true);
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
        Ciff::parse(ifile, 1, true);
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
        Ciff::parse(ifile, 1, true);
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
        Ciff::parse(ifile, 1, true);
        result = true;
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodHeaderSizeWithLimit1() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x40, 0xe2, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  //headerSize=123456
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2, true);
        if (c.getHeaderSize() == 123456) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodHeaderSizeWithLimit2() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=5242880
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2, true);
        if (c.getHeaderSize() == Ciff::MAX_CIFF_SIZE) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodHeaderSizeWithLimit3() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //headerSize=38
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2, true);
        if (c.getHeaderSize() == 38) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffBadHeaderSizeWithLimit1() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //headerSize=37 (MIN_HEADER_SIZE - 1)
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2, true);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid headerSize") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffBadHeaderSizeWithLimit2() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x01, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00 //headerSize=5242881
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2, true);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Ciff file can't exceed 5MB") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffBadHeaderSizeWithLimit3() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff //headerSize=18 446 744 073 709 551 615
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2, true);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Ciff file can't exceed 5MB") == 0) {
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
        Ciff c = Ciff::parse(ifile, 2, true);
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
                       0xff, 0xff, 0x00, 0x00
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 8, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 2, true);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: short headerSize") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffGoodContentSizeWithoutLimit1() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  //headerSize=18 446 744 073 709 551 615
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=0
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 20, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3, false);
        if (c.getContentSize() == 0) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodContentSizeWithoutLimit2() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  //headerSize=18 446 744 073 709 551 614
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=1
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 20, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3, false);
        if (c.getContentSize() == 1) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodContentSizeWithLimit1() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=5242880
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //contentSize=0
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 20, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3, true);
        if (c.getContentSize() == 0) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodContentSizeWithLimit2() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=38
                       0xda, 0xff, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, //contentSize=5242842
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 20, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3, true);
        if (c.getContentSize() == 5242842) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffBadContentSizeWithoutLimit1() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  //headerSize=18 446 744 073 709 551 615
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=1
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 20, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3, false);
    } catch (overflow_error& e) {
        if (string{e.what()}.compare("contentSize + headerSize should fit into 8 bytes") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffBadContentSizeWithoutLimit2() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //headerSize=38
                       0xda, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  //contentSize=18 446 744 073 709 551 615 - 37
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 20, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3, false);
    } catch (overflow_error& e) {
        if (string{e.what()}.compare("contentSize + headerSize should fit into 8 bytes") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffBadContentSizeWithLimit1() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=38
                       0xdb, 0xff, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, //contentSize=5242843
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 20, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3, true);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Ciff file can't exceed 5MB") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffBadContentSizeWithLimit2() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=5242880
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //contentSize=1
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 20, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3, true);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Ciff file can't exceed 5MB") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffNoContentSize() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=64
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 12, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3, true);
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
                       0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=64
                       0x0f, 0x0f, 0x0f, 0x0f
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 16, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 3, true);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: short contentSize") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffGoodWidthWithoutLimit() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,  //headerSize=4294967295
                       0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,  //contentSize=4294967295
                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  //width=unsigned long long max
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 28, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 4, false);
        if (c.getWidth() == numeric_limits<ull>::max()) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCiffGoodHeightCorrectContent() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,  //headerSize=4294967295
                       0x00, 0x40, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=3686400 (1200*1024*3)
                       0xb0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //width=1200
                       0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=1024
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 36, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 5, false);
        if (c.getWidth() == 1200 && c.getHeight() == 1024) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testGoodWidthHeightWithMaxCiffSize() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //headerSize=39
                       0xd8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  //contentSize=max - 39
                       0x48, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,  //width=(max - 39) / 3
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=1
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 36, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 5, false);
        if (c.getWidth() == (numeric_limits<ull>::max() - 39) / 3 && c.getHeight() == 1) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testWidthHeightOverflow1() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //headerSize=39
                       0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=3 (after overflow -> (max / 3 + 1) * 3)
                       0x56, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,  //width=max / 3 + 1
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=1
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 36, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 5, false);
    } catch (overflow_error& e) {
        if (string{e.what()}.compare("width * height * 3 should fit into 8 bytes") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testWidthHeightOverflow2() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //headerSize=39
                       0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=3 (after overflow -> (max / 3 + 1) * 3) * 3
                       0x56, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,  //width=max / 3 + 1
                       0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=1
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 36, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 5, false);
    } catch (overflow_error& e) {
        if (string{e.what()}.compare("width * height should fit into 8 bytes") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffGoodHeightWrongContent1() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, //headerSize=4294967295
                       0x01, 0x40, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=3686401 (1200*1024*3 + 1)
                       0x00, 0x0b, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,  //width=1200
                       0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=1024
                      }; 
    ifstream ifile;
    injectCiffToFile(testCiff, 36, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 5, false);
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
                       0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=38
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //width=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=0
                       '\n', //caption
                       '\0', //end of tags
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 38, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 6, true);
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
                       0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=43
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //width=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=0
                       'v', 'a', 'l', 'a', 'm', 'i', //caption
                       '\0', //end of tags
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 43, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 6, true);
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
                       0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=44
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //width=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=0
                       'v', 'a', 'l', 'a', 'm', 'i', '\n', //caption
                       '\0', //end of tags
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 44, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 6, true);
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
                       0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=43
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //width=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=0
                       'v', 'a', 'l', 'a', 'm', 'i', '\n', //caption
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 43, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 7, true);
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
                       0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //hederSize=64
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //width=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=0
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
        Ciff c = Ciff::parse(ifile, 7, true);
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
                       0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=63
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //width=0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=0
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
        Ciff c = Ciff::parse(ifile, 7, true);
        auto& tags = c.getTags();
        if (tags.size() == goodTags.size()) {
            for (ull i = 0; i < tags.size(); ++i) {
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
                       0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=63
                       0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=3
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //width=1
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=1
                       'v', 'a', 'l', 'a', 'm', 'i', '\n', //caption
                       't', 'a', 'g', '1', '\0',
                       't', 'a', 'g', '2', '\0',
                       't', 'a', 'g', '3', '\0',
                       't', 'a', 'g', '4', '\0',
                       0x01, 0x01,
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 65, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 8, true);
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
                       0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //headerSize=63
                       0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //contentSize=9
                       0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //width=3
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=1
                       'v', 'a', 'l', 'a', 'm', 'i', '\n', //caption
                       't', 'a', 'g', '1', '\0',
                       't', 'a', 'g', '2', '\0',
                       't', 'a', 'g', '3', '\0',
                       't', 'a', 'g', '4', '\0',
                       0x01, 0x02, 0x03,
                       0xff, 0xff, 0xff,
                       0x09, 0x0a, 0x0b,
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 72, ifile);
    bool result = true;
    vector<Pixel> goodPixels = { {1, 2, 3}, {255, 255, 255}, {9, 10, 11} };
    try {
        Ciff c = Ciff::parse(ifile, 8, true);
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
        //CIFF tests
        {&testCiffWithBadMagic, "testCiffWithBadMagic"},
        {&testCiffWithNoMagic, "testCiffWithNoMagic"},
        {&testCiffWithHalfMagic, "testCiffWithHalfMagic"},
        {&testCiffWithGoodMagic, "testCiffWithGoodMagic"},
        {&testCiffGoodHeaderSizeWithLimit1, "testCiffGoodHeaderSizeWithLimit1"},
        {&testCiffGoodHeaderSizeWithLimit2, "testCiffGoodHeaderSizeWithLimit2"},
        {&testCiffGoodHeaderSizeWithLimit3, "testCiffGoodHeaderSizeWithLimit3"},
        {&testCiffBadHeaderSizeWithLimit1, "testCiffBadHeaderSizeWithLimit1"},
        {&testCiffBadHeaderSizeWithLimit2, "testCiffBadHeaderSizeWithLimit2"},
        {&testCiffBadHeaderSizeWithLimit3, "testCiffBadHeaderSizeWithLimit3"},
        {&testCiffNoHeaderSize, "testCiffNoHeaderSize"},
        {&testCiffHalfHeaderSize, "testCiffHalfHeaderSize"},
        {&testCiffGoodContentSizeWithoutLimit1, "testCiffGoodContentSizeWithoutLimit1"},
        {&testCiffGoodContentSizeWithoutLimit2, "testCiffGoodContentSizeWithoutLimit2"},
        {&testCiffGoodContentSizeWithLimit1, "testCiffGoodContentSizeWithLimit1"},
        {&testCiffGoodContentSizeWithLimit2, "testCiffGoodContentSizeWithLimit2"},
        {&testCiffBadContentSizeWithoutLimit1, "testCiffBadContentSizeWithoutLimit1"},
        {&testCiffBadContentSizeWithoutLimit2, "testCiffBadContentSizeWithoutLimit2"},
        {&testCiffBadContentSizeWithLimit1, "testCiffBadContentSizeWithLimit1"},
        {&testCiffBadContentSizeWithLimit2, "testCiffBadContentSizeWithLimit2"},
        {&testCiffNoContentSize, "testCiffNoContentSize"},
        {&testCiffHalfContentSize, "testCiffHalfContentSize"},
        {&testCiffGoodWidthWithoutLimit, "testCiffGoodWidthWithoutLimit"},
        {&testCiffGoodHeightCorrectContent, "testCiffGoodHeightCorrectContent"},
        {&testGoodWidthHeightWithMaxCiffSize, "testGoodWidthHeightWithMaxCiffSize"},
        {&testWidthHeightOverflow1, "testWidthHeightOverflow1"},
        {&testWidthHeightOverflow2, "testWidthHeightOverflow2"},
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

    int successes = 0;
    for (auto& test : tests) {
        if (!test.first()) {
            cout << test.second << " failed" << endl;
        } else {
            ++successes;
            //cout << test.second << " succeeded" << endl;
        }
    }
    cout << "SUCCEEDED TESTS: " << successes << " FAILED TESTS: " << tests.size() - successes << endl;

    ifstream in;
    in.open("testfiles/1.caff", ifstream::in);
    try {
        Caff c1 = Caff::parse(in);
        cout << c1.getCreator() << endl;
    } catch (domain_error& e) {
        cout << e.what() << endl;
    }
    in.close();
    in.open("testfiles/2.caff", ifstream::in);
    try {
        Caff c2 = Caff::parse(in);
        cout << c2.getCreator() << endl;
        Ciff cc2 = c2.getCiff(0);
        ull bmpSize;
        auto bmp = cc2.getBMP(bmpSize);
        outputBmpToFile(bmp, bmpSize);
    } catch (exception& e) {
        cout << e.what() << endl;
    }
    in.close();
    in.open("testfiles/3.caff", ifstream::in);
    try {
        Caff c3 = Caff::parse(in);
    } catch (exception& e) {
        cout << e.what() << endl;
    }
    in.close();
}