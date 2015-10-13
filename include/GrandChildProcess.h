#ifndef _GRAND_CHILD_PROCESS_H
#define _GRAND_CHILD_PROCESS_H

#include <string>

#include <Process.h>

class GrandChildProcess : public Process {
private:
    std::string m_fileName;
    bool m_isFixedRent;

public:
    GrandChildProcess(const Pipe &pipe, const std::string &fileName,
            bool isFixedRent);
    virtual ~GrandChildProcess();
    virtual int execute();
};

#endif // _GRAND_CHILD_PROCESS_H
