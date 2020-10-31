#pragma once
#include "ciff.h"

class Caff
{
private:
    uint headerSize;
    uint numAnim;
    byte[] year;
    byte month;
    byte day;
    byte hour;
    byte minute;
    uint creatorLen;
    string creator;
    uint duration;
    vector<CIFF> ciffs;

    Caff(uint hs, uint na, byte[]& ye, byte mo, byte da, byte ho, byte mi, uint crlen, const string& cr, uint dur, const vector<CIFF>& ci):
        headerSize(hs), numAnim(na), year(ye), month(mo), day(da), hour(ho), minute(mi), creatorLen(crlen), creator(cr), duration(dur), ciffs(ci) {};

    Caff(){};
public:
    static const int MIN_HEADER_SIZE = 4 + 8 + 8;
    static const int MAIN_STAGES = 3;
    static const int HEADER_STAGES = 3;
    static const int CREDIT_STAGES = 7;
    static const int ANIMATION_STAGES = 2;

    inline uint getHeaderSize() { return headerSize; }
    inline uint getNumAnim() { return numAnim; }
    inline byte[] getYear() { return year; }
    inline byte getMonth() { return month; }
    inline byte getDay() { return day; }
    inline byte getHour() { return hour; }
    inline byte getMinute() { return minute; }
    inline uint getCreatorLen() { return creatorLen; }
    inline string getCreator() { return creator; }
    inline uint getDuration() { return duration; }

    CIFF getCIFF(uint);
    static Caff parse(istream&, int , int);
};

