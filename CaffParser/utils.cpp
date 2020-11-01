#include "utils.h"

uint Utils::intFromBytes(char* bytes, uint size) {
    uint res = 0;
    for (uint i = 0; i < size; ++i) {
        uint val = static_cast<byte>(bytes[i]);
        res = res | val << (size - i - 1) * 4;
    }
    return res;
}