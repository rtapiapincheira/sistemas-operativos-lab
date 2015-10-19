#include <MainProcess.h>
#include <ChildProcess.h>

#include <iomanip>

MainProcess::MainProcess() {
    m_level = 0;
    m_name = "MainProcess";
}

void MainProcess::assign(const Options &options) {
    m_options = options;
}

MainProcess::~MainProcess() {
    // No-op.
}

int MainProcess::execute() {
    // Execute parent process code first, to avoid duplication of it.
    m_ppid = getppid();
    m_pid = getpid();

    printStarted();

    std::vector<char> fundTypes = m_options.getFundTypes();

    // Spawn every child.
    size_t n = fundTypes.size();
    std::vector<ChildProcess> children(n);

    for (size_t i = 0; i < n; i++) {
        Pipe p;
        p.open();

        children[i].assign(p, m_options.getFileName(), m_options.hasFixedRent(),
                m_options.hasVariableRent(), fundTypes[i]);
    }

    m_children = asPointerVector<ChildProcess>(children);

    if (!makeFork()) {
        // Error
    }


    // Only for main process, collect every child response (child process won't
    // reach this far).
    _vstring finalResult = buildResponse();

    cout << flush << endl;
    cout << "Resultado Valorizacion:" << endl;
    cout << "=======================" << endl;
    cout << endl;
    for (size_t i = 0; i < finalResult.size(); i++) {
        _vstring parts = Utils::split(finalResult[i], ';');
        std::string _rf = parts[0];
        std::string _rv = parts[0];

        _vstring rf_parts = Utils::split(_rf, ',');
        _vstring rv_parts = Utils::split(_rv, ',');

        double rf = Utils::string2double(rf_parts[2]);
        double rv = Utils::string2double(rv_parts[2]);
        double sum = rf + rv;

        cout << "Total valorizacion tipo de fondo ";
        cout << "RF=" << std::fixed << std::setw(12) << std::setprecision(2) << rf << ", ";
        cout << "RV=" << std::fixed << std::setw(12) << std::setprecision(2) << rv << ", ";
        cout << "suma=" << std::fixed << std::setw(12) << std::setprecision(2) << sum << endl;
    }
    cout << endl << flush;


    printFinished();

    return 0;
}

_vstring MainProcess::buildResponse() {
    _vstring response;

    _vstring childrenResponse = Process::waitChildren();
    return childrenResponse;
    for (size_t i = 0; i < childrenResponse.size(); i++) {
        _vstring row_i;
        row_i.push_back("child#" + Utils::int2string(i));
        row_i.push_back(childrenResponse[i]);

        response.push_back(Utils::join(row_i, ' '));
    }

    return response;
}

