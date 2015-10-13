#include <iostream>

#include <Utils.h>

using namespace std;

int main(int argc, char **argv) {

    int val = 1234;

    _uchar buffer[2];
    Utils::ushort2bytes(val, buffer);

    cout << "bytes:" << (int)buffer[0] << "," << (int)buffer[1] << endl;

    _ushort u = Utils::bytes2ushort(buffer);

    cout << "_ushort:" << u << endl;

    return 0;
}
