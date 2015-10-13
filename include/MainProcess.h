#ifndef _MAIN_PROCESS_H
#define _MAIN_PROCESS_H

#include <Options.h>
#include <Process.h>

class MainProcess : public Process {
private:
    Options m_options;

public:
    MainProcess(const Options &options);
    virtual ~MainProcess();
    virtual int execute();
};

#endif // _MAIN_PROCESS_H
