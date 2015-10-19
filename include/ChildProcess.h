#ifndef _CHILD_PROCESS_H
#define _CHILD_PROCESS_H

#include <string>
#include <vector>
#include <GrandChildProcess.h>
#include <Process.h>

class ChildProcess : public Process {
private:
    std::string m_fileName;
    bool m_processFixedRent;
    bool m_processVariableRent;
    char m_fundType;

public:
    ChildProcess();
    void assign(const Pipe &pipe, const std::string &fileName,
            bool processFixedRent, bool processVariableRent,
            char fundType);
    virtual ~ChildProcess();

    virtual int execute();
    _vstring buildResponse();
};

#endif // _CHILD_PROCESS_H
