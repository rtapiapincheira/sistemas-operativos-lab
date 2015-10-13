#include <Utils.h>

#include <sstream>
#include <string>

std::string Utils::int2string(int x) {
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    std::string temp;
    ss << x;
    getline(ss, temp);
    return temp;
}

void Utils::ushort2bytes(_ushort x, _uchar *buffer) {
    size_t n = sizeof(_ushort);
    for (size_t i = 0; i < n; i++) {
        buffer[i] = (_uchar)((x >> (i*8)) & 0xFF);
    }
}
void Utils::int2bytes(int x, _uchar *buffer) {
    size_t n = sizeof(int);
    for (size_t i = 0; i < n; i++) {
        buffer[i] = (_uchar)((x >> (i*8)) & 0xFF);
    }
}

_ushort Utils::bytes2ushort(_uchar *buffer) {
    _ushort value = 0;
    size_t n = sizeof(_ushort);
    for (size_t i = 0; i < n; i++) {
        value |= (_ushort)(((_ushort)buffer[i]) << (i*8));
    }
    return value;
}
int Utils::bytes2int(_uchar *buffer) {
    int value = 0;
    size_t n = sizeof(int);
    for (size_t i = 0; i < n; i++) {
        value |= (int)(((int)buffer[i]) << (i*8));
    }
    return value;
}
