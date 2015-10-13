#ifndef _PROCESS_H
#define _PROCESS_H

#include <vector>

#include <Pipe.h>
#include <System.h>

class Process {
protected:
    int m_pid;
    System m_system;
    Pipe m_parentPipe;
    std::vector<Pipe> m_pipes;
    std::vector<Process*> m_subProcesses;

public:
    Process(const Pipe &pipe);
    virtual ~Process();

    virtual int execute()=0;
    int getPid() const;
    Pipe getPipe() const;
    std::vector<Pipe> getPipes() const;
};

#endif // _PROCESS_H
