#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <string>

#include <Pipe.h>

class System {
private:
    std::string m_lastError;

public:
    System();
    ~System();

    bool makeFork(int &pid) const;
    bool makePipe(Pipe &pipe) const;
    std::string getLastError() const;
};

#endif // _SYSTEM_H
