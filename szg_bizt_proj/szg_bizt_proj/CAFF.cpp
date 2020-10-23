#include "CAFF.h"

CAFF::CAFF() : magic(NULL), headerSize(0), NumberOfCIFFs(0), year(0), month(0), day(0), hour(0), minute(0), creatorLen(0), duration(0), images(nullptr) { /* empty image */ }
CAFF::CAFF(long long headerSize, long long NumberOfCIFFs, int year, int month, int day, int hour, int minute, long long creatorLen, std::string creator, long long duration, CIFF* imageArray)
{
    this->magic = { "CAFF" };
    this->headerSize = headerSize;
    this->NumberOfCIFFs = NumberOfCIFFs;
    this->year = year;
    this->month = month;
    this->day = day;
    this->hour = hour;
    this->minute = minute;
    this->creatorLen = creatorLen;
    this->creator = creator;
    this->duration = duration;
    images = new CIFF[NumberOfCIFFs];
    for (long long i = 0; i < NumberOfCIFFs; ++i) {
        images[i] = imageArray[i];
    }

}
const CIFF& CAFF::operator [] (const unsigned int& i) const
{
    return images[i];
}
CIFF& CAFF::operator [] (const unsigned int& i)
{
    return images[i];
}
CAFF::~CAFF()
{
    if (images != NULL) delete[] images;
}