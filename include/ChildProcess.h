#ifndef _CHILD_PROCESS_H
#define _CHILD_PROCESS_H

#include <GrandChildProcess.h>
#include <Process.h>

#include <string>
#include <vector>

/**
 * @brief The ChildProcess class defines the logic to be executed on a second
 * level process architecture (level=1). This class should have as its parent
 * the MainProcess(level=0) process, and will spawn subprocesses, instances of
 * GrandChildProcess(level=2) class.
 *
 * All the Process handling methods are inherited from Process, refer to that
 * class for the specifics.
 */
class ChildProcess : public Process {
private:
    /**
     * @brief m_fileName Name of the file to be processes by this Process and
     * its sub processes.
     */
    std::string m_fileName;
    /**
     * @brief m_processFixedRent Whether to process RF or not.
     */
    bool m_processFixedRent;
    /**
     * @brief m_processVariableRent Whether to process RV or not.
     */
    bool m_processVariableRent;
    /**
     * @brief m_fundType Type of fund to process (tipically one of 'A', 'B',
     * 'C', 'D' or 'E').
     */
    char m_fundType;

public:
    /**
     * @brief ChildProcess Constructor of this class, initializes some values.
     */
    ChildProcess();

    /**
     * @brief assign Sets initial parameters into this object (kind of a
     * constructor).
     * @param pipe Already open Pipe to set into this process, which will allow
     * communications between this process and its parent process.
     * @param fileName Name of the file to process.
     * @param processFixedRent Whether should process RF rent types.
     * @param processVariableRent Whether should process RV rent types.
     * @param fundType Fund to process by this ChildProcess (tipically one out
     * of: 'A', 'B', 'C', 'D', 'E'.
     */
    void assign(const Pipe &pipe, const std::string &fileName,
            bool processFixedRent, bool processVariableRent, char fundType);

    /**
     * @brief ~ChildProcess Deallocates resources for this instance (no-op,
     * since we're not using dynamic memory).
     */
    virtual ~ChildProcess();

    /**
     * @brief execute Dictates which subprocess should be run to calculate the
     * RF and/or RV rent business rules. Also gathers the results from those
     * subprocesses and pumps it out into the parent process of this process.
     * @return 0 if the processing was ok, other value if not (by contract).
     */
    virtual int execute();

    /**
     * @brief buildResponse Waits the response from all the children of this
     * process and collects it to communicate it to the parent process of this
     * process.
     * @return Vector of strings, one for each child of this process (tipically
     * 1 or 2).
     */
    _vstring buildResponse();
};

#endif // _CHILD_PROCESS_H
