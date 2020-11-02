#include "utils.h"

ull Utils::intFromBytes(char* bytes, ull size) {
    ull res = 0;
    for (ull i = 0; i < size; ++i) {
        ull val = static_cast<byte>(bytes[i]);
        res = res | val << (i * 8);
    }
    return res;
}

void Utils::fillWithIntToBytes(byte* buff, ull num, int from, int siz) {
	for (int i = 0; i < siz; ++i) {
		buff[i + from] = (num >> (i * 8));
	}
}