#include <ChildProcess.h>

ChildProcess::ChildProcess() {
    m_level = 1;
    m_name = "ChildProcess";
}

void ChildProcess::assign(const Pipe &pipe, const std::string &fileName,
        bool processFixedRent, bool processVariableRent, char fundType)
{
    Process::assign(pipe);
    m_fileName = fileName;
    m_processFixedRent = processFixedRent;
    m_processVariableRent = processVariableRent;
    m_fundType = fundType;
}
ChildProcess::~ChildProcess() {
    // No-op.
}

int ChildProcess::execute() {

    printStarted();

    std::vector<GrandChildProcess> children;

    if (m_processFixedRent) {
        Pipe p;
        p.open();
        GrandChildProcess gcp;
        gcp.assign(p, m_fileName, m_fundType, true);
        children.push_back(gcp);
    }

    if (m_processVariableRent) {
        Pipe p;
        p.open();
        GrandChildProcess gcp;
        gcp.assign(p, m_fileName, m_fundType, false);
        children.push_back(gcp);
    }

    m_children = asPointerVector<GrandChildProcess>(children);

    if (!makeFork()) {
        // Error.
        return 1;
    }

    // Child processes won't reach this far.
    _vstring res = buildResponse();

    std::string content = Utils::join(res, ';');
    m_parentPipe.writeString(content);

    //cout << "  Child writes content=" << content << endl;

    m_parentPipe.closeAll();

    printFinished();

    return 0;
}

_vstring ChildProcess::buildResponse() {
    return Process::waitChildren();
}

