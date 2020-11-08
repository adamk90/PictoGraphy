#pragma once
#include "ciff.h"

struct Credits {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	string creator;
};

struct ParsingStatus {
	bool headerDone;
	bool creditsDone;
	ull numAnims;
	ull processedAnims;

	ParsingStatus() : headerDone(false), creditsDone(false), numAnims(0), processedAnims(0) {}
};

struct Animation {
	ull duration;
	Ciff img;

	Animation(): duration(0) {};
};

class Caff {
private:
    Credits credits;
    vector<Animation> anims;

    Caff(Credits c, const vector<Animation>& an):
        credits(c), anims(an) {};

    static void parseBlock(istream&, ull&, ParsingStatus&, Credits&, vector<Animation>&);
    static ull parseHeader(istream&, const ull&);
    static void parseCredits(istream&, const ull&, Credits&);
    static void parseAnimation(istream&, const ull&, vector<Animation>&);

public:
	static const uint HEADER_SIZE = 20;
	static const uint MIN_CREDITS_SIZE = 14;
	static const uint MIN_ANIMATION_SIZE = Ciff::MIN_HEADER_SIZE + 8;
    static const ull MAX_CAFF_SIZE = 209715200;

    inline int getYear() const { return credits.year; }
    inline int getMonth() const { return credits.month; }
    inline int getDay() const { return credits.day; }
    inline int getHour() const { return credits.hour; }
    inline int getMinute() const { return credits.minute; }
    inline string getCreator() const { return credits.creator; }
    inline const vector<Animation>& getAnimations() const { return anims; }
    
    static Caff parse(istream&);
};