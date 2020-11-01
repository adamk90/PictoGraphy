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

typedef unsigned int uint;
typedef unsigned char byte;
typedef unsigned long long ull;

using namespace std;

class Utils {
public:
	Utils() = delete;

	static uint intFromBytes(char*, uint);
};