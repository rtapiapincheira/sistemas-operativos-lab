#ifndef _PROCESS_H
#define _PROCESS_H

#include <vector>

#include <Pipe.h>

/**
 * @brief The Process class is the entity representation of a running process.
 * This class encapsulates all the logic for handling different processes and
 * the related methods for forking new sub processes and collecting them (at the
 * moment, the only communication available is from the subprocess to the
 * process, in form of complete std::strings).
 *
 * It also contains a Pipe to its parent process, which serves for communicating
 * results of the processing of this particular instance.
 *
 * This class is modeled as a pure virtual class, in order to allocate the
 * specific processing logic on subclasses, but every sub class should
 * explicitely allocate subprocesses and start them by using the methods of this
 * base class.
 */
class Process {
public://protected:
    /**
     * @brief m_ppid Parent process id of this process.
     */
    pid_t m_ppid;
    /**
     * @brief m_pid Process id of this process.
     */
    pid_t m_pid;

    /**
     * @brief m_parentPipe Pipe shared between this process and its parent
     * process.
     */
    Pipe m_parentPipe;

    /**
     * @brief m_level For formatting purposes, tells whether it is the main
     * process (level = 0), child process (level = 1) or the grand child process
     * (level = 2).
     */
    int m_level;
    /**
     * @brief m_name For formmating purposes, name of the derived class for
     * which this instance comes from.
     */
    std::string m_name;

    /**
     * @brief m_children Vector for the addresses of the sub-processes this
     * process should be parent of.
     */
    std::vector<Process*> m_children;

public:
    /**
     * @brief Process Constructor of this class, initializes some values.
     */
    Process();

    /**
     * @brief assign Sets initial parameters into this object (kind of a
     * constructor).
     * @param pipe Already open Pipe to set into this process, which will allow
     * communications between this process and its parent process.
     */
    void assign(const Pipe &pipe);

    /**
     * @brief ~Process Deallocates stuff (since we're not using dynamic memory,
     * this is no-op.
     */
    virtual ~Process();

    /**
     * @brief execute Represents the logic to be run in a particular process
     * derived from this class. Put here the logic to create and spawn
     * subprocesses of this process.
     * @return Exit code for the process this object represents (if the
     * processing is ok, it should return 0 as that's the contract).
     */
    virtual int execute()=0;

    /**
     * @brief makeFork Spawns the subprocesses already set to m_children. Make
     * sure to set the already the children to fork in the m_children vector
     * (with help of asPointerVector) before calling this method (or it won't
     * fork nothing). The subprocesses should have assigned it's respective
     * Pipes so as to allow communication from subprocesses into this current
     * process.
     * @return true when all the forks are ok, otherwise false.
     */
    bool makeFork();

    /**
     * @brief waitChildren Blocks the current process until all the children
     * have finished. Make sure to set the already forked children in the
     * m_children array (with help of asPointerVector) before calling this
     * method (or else, this process will wait on garbage, possibly blocking
     * itself until an unrelated process finishes).
     * @return a vector of strings, with all the chars read from every child.
     */
    _vstring waitChildren();

    /**
     * @brief isChildFinished Tells whether the sub process whose pid is passed
     * as argument, has exited and it's ready to be collected.
     * @param pid Pid for the sub process to check whether has finished or not.
     * @return true if the subprocess has finished, otherwise false.
     */
    bool isChildFinished(pid_t pid);

    /**
     * Gets a vector of addresses for a generic vector. It's important to use
     * a vector of elements whose class (T) derives from Process or this will
     * throw compile-time warnings (since the pointer types won't match). This
     * is useful for accesing the heap-allocated elements using the Process*
     * interface, while using the polimorphism of T and Process.
     */
    template <class T>
    static std::vector<Process*> asPointerVector(std::vector<T> &v) {
        size_t n = v.size();
        std::vector<Process*> res(n);
        for (size_t i = 0; i < n; i++) {
            res[i] = &v[i];
        }
        return res;
    }

    /**
     * @brief printStarted Prints a start debugging information, with info
     * related to this process (pid, ppid, class name, etc).
     */
    void printStarted();

    /**
     * @brief printFinished Prints a finished debugging information, with info
     * related to this process (pid, ppid, class name, etc).
     */
    void printFinished();
};

#endif // _PROCESS_H
