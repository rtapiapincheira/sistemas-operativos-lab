#ifndef _CHILD_PROCESS_H
#define _CHILD_PROCESS_H

#include <string>
#include <vector>

#include <Process.h>

class ChildProcess : public Process {
private:
    std::string m_fileName;
    bool m_processFixedRent;
    bool m_processVariableRent;
    char m_fundTypeToProcess;

public:
    ChildProcess(const Pipe &pipe, const std::string &fileName,
            bool processFixedRent, bool processVariableRent,
            char fundTypeToProcess);
};

#endif // _CHILD_PROCESS_H
