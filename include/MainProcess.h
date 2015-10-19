#ifndef _MAIN_PROCESS_H
#define _MAIN_PROCESS_H

#include <Options.h>
#include <Process.h>
#include <ChildProcess.h>

/**
 * @brief The MainProcess class defines the logic to be executed on a first
 * level process architecture (level=0). This class should not have a particular
 * parent process, because it's the entry point for the executable and will
 * spawn subprocesses, instances of ChildProcess(level=1) class.
 */
class MainProcess : public Process {
private:
    /**
     * @brief m_options Options parsed from the command line, arguments supplied
     * to the main function.
     */
    Options m_options;

public:
    /**
     * @brief MainProcess Constructor of this class, initializes some values.
     */
    MainProcess();

    /**
     * @brief assign Sets initial parameters into this object (kind of a
     * constructor).
     * @param options Already parsed command line arguments of the main process.
     * Based on values of this object, subprocesses will be built and spawn.
     */
    void assign(const Options &options);

    /**
     * @brief ~MainProcess Deallocates resources for this instance (no-op, since
     * we're not using dynamic memory).
     */
    virtual ~MainProcess();

    /**
     * @brief execute Contains the logic for spawning 1 or 5 subprocesses
     * (depending on whether a single fund type was specified or none, in which
     * case it will spawn all of them). It also waits for the children to finish
     * and builds the final response which will be sent to the final user of
     * this application.
     * @return 0 if the processing was ok, other value if not (by contract).
     */
    virtual int execute();

    /**
     * @brief buildResponse Waits the response from all the children of this
     * process and collects it to communicate it to the parent process of this
     * process.
     * @return Vector of strings, one for each child of this process (tipically
     * 1 or 5).
     */
    _vstring buildResponse();
};

#endif // _MAIN_PROCESS_H
