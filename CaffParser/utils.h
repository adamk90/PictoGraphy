#pragma once
#include <limits>
#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <iostream>
#include <memory>
#include <utility>
#include <functional>

typedef unsigned long long ull;
typedef unsigned char byte;

using namespace std;

class Utils {
public:
	Utils() = delete;

	static ull intFromBytes(char*, ull);
	static void fillWithIntToBytes(byte*, ull, int, int);
};