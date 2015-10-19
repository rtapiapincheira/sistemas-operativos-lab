#include <Process.h>

#include <sys/wait.h>

Process::Process() :
    m_ppid(-1),
    m_pid(-1),
    m_level(0),
    m_name("")
{

}
void Process::assign(const Pipe &pipe)
{
    m_ppid = -1;
    m_pid = -1;
    m_parentPipe = pipe;
}

Process::~Process() {

}

bool Process::makeFork() {
    for (size_t i = 0; i < m_children.size(); i++) {
        Process *p = m_children[i];

        pid_t childPid = fork();

        if (childPid == 0) {
            // Child process
            p->m_ppid = getppid(); // It should be the same as m_pid
            p->m_pid = getpid();

            p->m_parentPipe.closeReader();
            int code = p->execute();

            // It's important for every children to exit or will begin to
            // execute main process code which is duplicated!
            exit(code);
        } else if (childPid < 0) {
            // Error while forking.
            exit(-1);
        } else {
            p->m_parentPipe.closeWriter();
            p->m_pid = childPid;
            p->m_ppid = getpid();
        }
    }
    return true;
}

_vstring Process::waitChildren() {
    size_t n = m_children.size();
    _vstring output(n);

    std::vector<bool> finished(n);
    size_t numFinished = 0;

    // Read all the strings from the pipe, until all processes are finished.
    while(numFinished < n) {

        for (size_t i = 0; i < n; i++) {
            Pipe &pi = m_children[i]->m_parentPipe;

            if (!finished[i]) {

                std::string temp;
                if (pi.maybeReadString(temp)) {
                    output[i] = temp;
                }

                if (isChildFinished(m_children[i]->m_pid)) {
                    finished[i] = true;
                    numFinished++;
                }
            }
        }
    }
    return output;
}

bool Process::isChildFinished(pid_t pid) {
    int status = 0;
    pid_t wpid = waitpid(pid, &status, WNOHANG);
    return (wpid != 0 && WIFEXITED(status) != 0);
}

void Process::printStarted() {
    std::string buffer;
    for (int i = 0; i < m_level; i++)  {
        buffer += "  ";
    }
    buffer += "Executing ";
    buffer += m_name;
    buffer += "(pid:";
    buffer += Utils::int2string(m_pid);
    buffer += ", ppid:";
    buffer += Utils::int2string(m_ppid);
    buffer += ")";

    std::cout << buffer << std::endl;
}
void Process::printFinished() {
    std::string buffer;
    for (int i = 0; i < m_level; i++) {
        buffer += "  ";
    }
    buffer += "Ended ";
    buffer += m_name;
    buffer += "(pid:";
    buffer += Utils::int2string(m_pid);
    buffer += ", ppid:";
    buffer += Utils::int2string(m_ppid);
    buffer += ")";

    std::cout << buffer << std::endl;
}
