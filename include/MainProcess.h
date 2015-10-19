#ifndef _MAIN_PROCESS_H
#define _MAIN_PROCESS_H

#include <Options.h>
#include <Process.h>
#include <ChildProcess.h>

class MainProcess : public Process {
private:
    Options m_options;

public:
    MainProcess();

    void assign(const Options &options);

    virtual ~MainProcess();
    virtual int execute();

    _vstring buildResponse();
};

#endif // _MAIN_PROCESS_H
