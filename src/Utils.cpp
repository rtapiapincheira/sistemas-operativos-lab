#include <Utils.h>

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

_vstring Utils::split(const std::string &line, char sep) {
    _vstring res;
    if (line.empty()) {
        return res;
    }
    std::string buffer;
    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];
        if (c == sep) {
            res.push_back(buffer);
            buffer.clear();
        } else {
            buffer.push_back(c);
        }
    }
    res.push_back(buffer);
    return res;
}

std::string Utils::join(const _vstring &v, char sep) {
    std::string response;
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) {
            response += sep;
        }
        response += v[i];
    }
    return response;
}
#include <iostream>
using namespace std;

std::string Utils::trim(const std::string &s, const std::string &chars) {
    int start = 0;
    while(chars.find(s[start]) != std::string::npos) {
        start++;
    }
    int last = s.size()-1;
    while(chars.rfind(s[last]) != std::string::npos && last >= start) {
        last--;
    }
    if (last < start) {
        return "";
    }
    return s.substr(start, last-start+1);
}

double Utils::string2double(const std::string &s, bool *ok) {
    bool foo;
    if (!ok) {
        ok = &foo;
    }
    std::string t = trim(s);
    if (t == "") {
        return false;
    }
    char* p;
    double converted = strtod(t.c_str(), &p);
    // Ok only if the remaining is empty.
    *ok = (trim(std::string(p)) == "" ? true : false);
    if (*ok) {
        return converted;
    }
    return 0.0;
}

int Utils::string2int(const std::string &s, bool *ok) {
    bool foo;
    if (!ok) {
        ok = &foo;
    }
    std::string t = trim(s);
    if (t == "") {
        return false;
    }
    char* p;
    int converted = ::strtol(t.c_str(), &p, 10);
    // Ok only if the remaining is empty.
    *ok = (trim(std::string(p)) == "" ? true : false);
    if (*ok) {
        return converted;
    }
    return 0;
}

std::string Utils::string2upper(const std::string &s) {
    size_t n = s.size();
    std::string temp(n, '\0');
    for (size_t i = 0; i < n; i++) {
        temp[i] = toupper(s[i]);
    }
    return temp;
}

std::string Utils::double2string(double x) {
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    std::string temp;
    ss << x;
    getline(ss, temp);
    return temp;
}
