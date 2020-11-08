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

void outputBmpToFile(const string& filePath, shared_ptr<byte> bmp, ull bmpSize) {
    ofstream ofile;
    ofile.open(filePath, ofstream::out | ofstream::trunc | ofstream::binary);
    ofile.write(reinterpret_cast<const char *>(bmp.get()), bmpSize);
    ofile.close();
}

void dumpCaff(const Caff& c1, const string& previewName) {
    cout << "Creator: " << c1.getCreator() << endl;
    cout << "Year: " << c1.getYear();
    cout << " Month: " << c1.getMonth();
    cout << " Day: " << c1.getDay();
    cout << " Hour: " << c1.getHour();
    cout << " Minute: " << c1.getMinute() << endl;
    vector<Animation> anims = c1.getAnimations();
    for (ull i = 0; i < anims.size(); ++i) {
        cout << "Animation " << i + 1 << " duration: " << anims[i].duration << endl;
        ull bmpSize = 0;
        auto bmpBytes = anims[i].img.getBMP(bmpSize);
        string name = "test_previews/" + previewName + "_" + to_string(i) + ".bmp";
        outputBmpToFile(name, bmpBytes, bmpSize);
    }
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

bool testCiffNoWidth() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,  //headerSize=4294967295
                       0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,  //contentSize=4294967295
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 20, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 4, false);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: short width") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCiffNoHeight() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,  //headerSize=4294967295
                       0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,  //contentSize=4294967295
                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  //width=unsigned long long max
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 28, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 5, false);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: short height") == 0) {
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

bool testInvalidHeaderContentSize() {
    byte testCiff[] = {'C', 'I', 'F', 'F', //magic
                       0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //headerSize=39
                       0xd8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  //contentSize=max - 39
                       0x48, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,  //width=(max - 39) / 3
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //height=1
                       0x00
                      };
    ifstream ifile;
    injectCiffToFile(testCiff, 37, ifile);
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 6, false);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid CIFF format: invalid headerSize") == 0) {
            result = true;
        }
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

bool testCiffGettingWrongPixel() {
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
    bool result = false;
    try {
        Ciff c = Ciff::parse(ifile, 8, true);
        c.getPixel(9);
    } catch (domain_error& e) {
    } catch (out_of_range& e) {
        if (string{e.what()}.compare("Index is out of bound") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffShortLength() {
    byte testCaff[] = {0x01, // CAFF block id
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //short
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 8, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff block: short length") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffEmptyCaff() {
    byte testCaff[] = {};
    ifstream ifile;
    injectCiffToFile(testCaff, 0, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff: empty") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffShortHeaderSize() {
    byte testCaff[] = {0x01, // CAFF block id
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'f', //wrong magic
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 13, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff header: actual size is smaller than declared") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffBadMagic() {
    byte testCaff[] = {0x01, // CAFF block id
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'f', //wrong magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 29, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff header: wrong magic") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffInvalidHeaderSize1() {
    byte testCaff[] = {0x01, // CAFF block id
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 19
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 29, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff header: size should be 20") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffInvalidHeaderSize2() {
    byte testCaff[] = {0x01, // CAFF block id
                       0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 19
                       'C', 'A', 'F', 'F', //good magic
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 13, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff header: size should be 20") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffHeaderShouldBeFirst1() {
    byte testCaff[] = {0x02, // CAFF block id
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 1, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff: the first block must be the header") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffHeaderShouldBeFirst2() {
    byte testCaff[] = {0x03, // CAFF block id
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 1, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff: the first block must be the header") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffNoSuchBlockID() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x00, // CAFF block id -> no such block id
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 38, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff block: no such ID: 0") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffMultipleHeaders() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 58, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff: multiple headers found") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffShortAnimationSize() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x03, // CAFF block id -> animation
                       0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length 46
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 38, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff animation: actual size is smaller than declared") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffInvalidAnimationSize() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x03, // CAFF block id -> animation
                       0x2d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //fewer bytes than 8 + 38 (min ciff header)
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 38, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff animation: size invalid") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffTooBigFile1() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x03, // CAFF block id -> animation
                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //length too big
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 38, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (overflow_error& e) {
        if (string{e.what()}.compare("Invalid Caff: length overflow") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffTooBigFile2() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x03, // CAFF block id -> animation
                       0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //length too big
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 38, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff: can't be more than 200 MB") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffNoCreditsBlock() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       //MISSING ID
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 29, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff: no credits block found") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffInvalidCredits() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x02, // CREDITS
                       0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //smaller than min credits length
                       0xd7, 0x07, //2007
                       0x07, //july
                       0x14, //20
                       0x0c, //12
                       0x00, //0 ->> 2000.07.20 12:00
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 44, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff credits: size should be at least 14") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffShortCredits() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x02, // CREDITS
                       0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //min credits length
                       0xd7, 0x07, //2007
                       0x07, //july
                       0x14, //20
                       0x0c, //12
                       0x00, //0 ->> 2000.07.20 12:00
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 44, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff credits: actual size is smaller than declared") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffGoodCredits() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x02, // CREDITS
                       0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //min credits length
                       0xd7, 0x07, //2007
                       0x07, //july
                       0x14, //20
                       0x0c, //12
                       0x00, //0 ->> 2000.07.20 12:00
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 52, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
        if (c.getYear() == 2007 && c.getMonth() == 7 && c.getDay() == 20 && c.getHour() == 12 && c.getMinute() == 0 && c.getCreator().compare(string{""}) == 0) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testCaffMultipleCredits() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x02, // CREDITS
                       0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //min credits length
                       0xd7, 0x07, //2007
                       0x07, //july
                       0x14, //20
                       0x0c, //12
                       0x00, //0 ->> 2000.07.20 12:00
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                       0x02, // CREDITS
                       0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //min credits length
                       0xd7, 0x07, //2007
                       0x07, //july
                       0x14, //20
                       0x0c, //12
                       0x00, //0 ->> 2000.07.20 12:00
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 75, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff: multiple credits found") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffInvalidCreatorLength1() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x02, // CREDITS
                       0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //min credits length
                       0xd7, 0x07, //2007
                       0x07, //july
                       0x14, //20
                       0x0c, //12
                       0x00, //0 ->> 2000.07.20 12:00
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //creator length = 1
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 52, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff credits: actual size does not match declared size") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffInvalidCreatorLength2() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x02, // CREDITS
                       0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //min credits length + 3
                       0xd7, 0x07, //2007
                       0x07, //july
                       0x14, //20
                       0x0c, //12
                       0x00, //0 ->> 2000.07.20 12:00
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //creator length = 1
                       0x00, 0x00,
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 54, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff credits: actual size does not match declared size") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffCreatorLengthTooBig() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x02, // CREDITS
                       0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //min credits length + 3
                       0xd7, 0x07, //2007
                       0x07, //july
                       0x14, //20
                       0x0c, //12
                       0x00, //0 ->> 2000.07.20 12:00
                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //creator length too big
                       0x00, 0x00,
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 54, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff credits: creatorLen too big") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testCaffGoodCreator() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0
                       0x02, // CREDITS
                       0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //min credits length + 3
                       0xd7, 0x07, //2007
                       0x07, //july
                       0x14, //20
                       0x0c, //12
                       0x00, //0 ->> 2000.07.20 12:00
                       0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //creator length = 3
                       'I', 'D', 'A',
                      };
    ifstream ifile;
    injectCiffToFile(testCaff, 55, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
        if (c.getCreator().compare(string{"IDA"}) == 0) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testGoodAnimation() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 1
                       0x02, // CREDITS
                       0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //min credits length + 3
                       0xd7, 0x07, //2007
                       0x07, //july
                       0x14, //20
                       0x0c, //12
                       0x00, //0 ->> 2000.07.20 12:00
                       0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //creator length = 3
                       'I', 'D', 'A',
                       0x03, //ANIMATION
                       0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //block size = 80
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //anim duration
                       'C', 'I', 'F', 'F', //magic
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
    injectCiffToFile(testCaff, 144, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
        vector<Animation> anims = c.getAnimations();
        if (c.getCreator().compare(string{"IDA"}) == 0 && anims.size() == 1) {
            result = true;
        }
    } catch (domain_error& e) {
    }
    ifile.close();
    return result;
}

bool testProcessedMoreAnimations() {
    byte testCaff[] = {0x01, // CAFF HEADER
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //CAFF block length = 20
                       'C', 'A', 'F', 'F', //good magic
                       0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF header size = 20
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //CAFF animation num = 0 !!!
                       0x02, // CREDITS
                       0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //min credits length + 3
                       0xd7, 0x07, //2007
                       0x07, //july
                       0x14, //20
                       0x0c, //12
                       0x00, //0 ->> 2000.07.20 12:00
                       0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //creator length = 3
                       'I', 'D', 'A',
                       0x03, //ANIMATION
                       0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //block size = 80
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //anim duration
                       'C', 'I', 'F', 'F', //magic
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
    injectCiffToFile(testCaff, 144, ifile);
    bool result = false;
    try {
        Caff c = Caff::parse(ifile);
    } catch (domain_error& e) {
        if (string{e.what()}.compare("Invalid Caff: more animations present than provided in header") == 0) {
            result = true;
        }
    }
    ifile.close();
    return result;
}

bool testFullCaff1() {
    bool result = true;
    ifstream in;
    in.open("testfiles/1.caff", ifstream::in);
    try {
        cout << "Testing 1.caff - should be valid!" << endl;
        Caff c1 = Caff::parse(in);
        dumpCaff(c1, "preview1");
    } catch (exception& e) {
        result = false;
    }
    in.close();
    return result;
}

bool testFullCaff2() {
    bool result = true;
    ifstream in;
    in.open("testfiles/2.caff", ifstream::in);
    try {
        cout << "Testing 2.caff - should be valid!" << endl;
        Caff c1 = Caff::parse(in);
        dumpCaff(c1, "preview2");
    } catch (exception& e) {
        result = false;
    }
    in.close();
    return result;
}

bool testFullCaff3() {
    bool result = false;
    ifstream in;
    in.open("testfiles/3.caff", ifstream::in);
    try {
        cout << "Testing 3.caff - should be invalid!" << endl;
        Caff c1 = Caff::parse(in);
    } catch (exception& e) {
        result = true;
    }
    in.close();
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
        {&testCiffNoWidth, "testCiffNoWidth"},
        {&testCiffNoHeight, "testCiffNoHeight"},
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
        {&testInvalidHeaderContentSize, "testInvalidHeaderContentSize"},
        {&testCiffGettingWrongPixel, "testCiffGettingWrongPixel"},
        {&testCiffPixels, "testCiffPixels"},
        //CAFF tests
        {&testCaffShortLength, "testCaffShortLength"},
        {&testCaffShortHeaderSize, "testCaffShortHeaderSize"},
        {&testCaffEmptyCaff, "testCaffEmptyCaff"},
        {&testCaffBadMagic, "testCaffBadMagic"},
        {&testCaffInvalidHeaderSize1, "testCaffInvalidHeaderSize1"},
        {&testCaffInvalidHeaderSize2, "testCaffInvalidHeaderSize2"},
        {&testCaffHeaderShouldBeFirst1, "testCaffHeaderShouldBeFirst1"},
        {&testCaffHeaderShouldBeFirst2, "testCaffHeaderShouldBeFirst2"},
        {&testCaffNoSuchBlockID, "testCaffNoSuchBlockID"},
        {&testCaffInvalidAnimationSize, "testCaffInvalidAnimationSize"},
        {&testCaffShortAnimationSize, "testCaffShortAnimationSize"}, 
        {&testCaffTooBigFile1, "testCaffTooBigFile1"},
        {&testCaffTooBigFile2, "testCaffTooBigFile2"},
        {&testCaffNoCreditsBlock, "testCaffNoCreditsBlock"},
        {&testCaffInvalidCredits, "testCaffInvalidCredits"},
        {&testCaffShortCredits, "testCaffShortCredits"},
        {&testCaffGoodCredits, "testCaffGoodCredits"},
        {&testCaffMultipleCredits, "testCaffMultipleCredits"},
        {&testCaffMultipleHeaders, "testCaffMultipleHeaders"},
        {&testCaffInvalidCreatorLength1, "testCaffInvalidCreatorLength1"},
        {&testCaffInvalidCreatorLength2, "testCaffInvalidCreatorLength2"},
        {&testCaffCreatorLengthTooBig, "testCaffCreatorLengthTooBig"},
        {&testCaffGoodCreator, "testCaffGoodCreator"},
        {&testGoodAnimation, "testGoodAnimation"},
        {&testProcessedMoreAnimations, "testProcessedMoreAnimations"},
        //Full CAFFs
        {&testFullCaff1, "testFullCaff1"},
        {&testFullCaff2, "testFullCaff2"},
        {&testFullCaff3, "testFullCaff3"},
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
}