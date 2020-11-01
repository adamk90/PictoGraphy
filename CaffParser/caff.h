#pragma once
#include "ciff.h"

/*
class Caff
{
private:
    ull headerSize;
    ull numAnim;
    byte[] year;
    byte month;
    byte day;
    byte hour;
    byte minute;
    ull creatorLen;
    string creator;
    ull duration;
    vector<CIFF> ciffs;

    Caff(ull hs, ull na, byte[]& ye, byte mo, byte da, byte ho, byte mi, ull crlen, const string& cr, ull dur, const vector<CIFF>& ci):
        headerSize(hs), numAnim(na), year(ye), month(mo), day(da), hour(ho), minute(mi), creatorLen(crlen), creator(cr), duration(dur), ciffs(ci) {};

    Caff(){};
public:
    static const int MIN_HEADER_SIZE = 4 + 8 + 8;
    static const int MAIN_STAGES = 3;
    static const int HEADER_STAGES = 3;
    static const int CREDIT_STAGES = 7;
    static const int ANIMATION_STAGES = 2;

    inline ull getHeaderSize() { return headerSize; }
    inline ull getNumAnim() { return numAnim; }
    inline byte[] getYear() { return year; }
    inline byte getMonth() { return month; }
    inline byte getDay() { return day; }
    inline byte getHour() { return hour; }
    inline byte getMinute() { return minute; }
    inline ull getCreatorLen() { return creatorLen; }
    inline string getCreator() { return creator; }
    inline ull getDuration() { return duration; }

    CIFF getCIFF(ull);
    static Caff parse(istream&, int , int);
};
*/
