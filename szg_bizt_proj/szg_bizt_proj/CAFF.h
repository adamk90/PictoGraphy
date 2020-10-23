#pragma once
#include "CIFF.h"

class CAFF
{
private:
	const char* magic;
	long long headerSize;
	long long NumberOfCIFFs;
	int year, month, day, hour, minute;
	long long creatorLen;
	std::string creator;
	long long duration;
	CIFF* images;

public:
	CAFF();
	CAFF(long long headerSize, long long NumberOfCIFFs, int year, int month, int day, int hour, int minute, long long creatorLen, std::string creator, long long duration, CIFF* imageArray);
	const CIFF& operator [] (const unsigned int& i) const;
	CIFF& operator [] (const unsigned int& i);
	~CAFF();
};

