#ifndef _PROCESS_H
#define _PROCESS_H

#include <vector>

#include <Pipe.h>

// Exit codes for processes.
#define PROCESS_EXIT_OK    0
#define PROCESS_BAD_ARG    1
#define PROCESS_ERROR_FILE 2
#define PROCESS_ERROR_FORK 3
#define PROCESS_ERROR_PIPE 4

#include <iostream>
using namespace std;

class Process {
public://protected:
    pid_t m_ppid;
    pid_t m_pid;
    Pipe m_parentPipe;

    int m_level;
    std::string m_name;

    std::vector<Process*> m_children;

public:
    Process();
    void assign(const Pipe &pipe);
    virtual ~Process();

    virtual int execute()=0;

    bool makeFork();

    _vstring waitChildren();

    bool isChildFinished(pid_t pid);

    template <class T>
    static std::vector<Process*> asPointerVector(std::vector<T> &v) {
        size_t n = v.size();
        std::vector<Process*> res(n);
        for (size_t i = 0; i < n; i++) {
            res[i] = &v[i];
        }
        return res;
    }

    void printStarted();
    void printFinished();
};

#endif // _PROCESS_H
