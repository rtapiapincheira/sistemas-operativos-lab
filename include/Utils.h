#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>

typedef unsigned short _ushort;
typedef unsigned char _uchar;

class Utils {
public:
    static std::string int2string(int x);

    static void ushort2bytes(_ushort x, _uchar *buffer);
    static void int2bytes(int x, _uchar *buffer);

    static _ushort bytes2ushort(_uchar *buffer);
    static int bytes2int(_uchar *buffer);
};

#endif // _UTILS_H_
