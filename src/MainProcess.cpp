#include <MainProcess.h>

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
        return 1;
    }


    // Only for main process, collect every child response (child process won't
    // reach this far).
    _vstring finalResult = buildResponse();

    std::cout << std::flush << std::endl;
    std::cout << "Resultado Valorizacion:" << std::endl;
    std::cout << "=======================" << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < finalResult.size(); i++) {
        _vstring parts = Utils::split(finalResult[i], ';');

        std::cout << "Total valorizacion tipo de fondo " << (char)('A'+i) << ":  ";

        double sum = 0.0;
        for (size_t j = 0; j < parts.size(); j++) {
            std::string _r = parts[j];
            _vstring r_parts = Utils::split(_r, ',');
            double r = Utils::string2double(r_parts[2]);
            sum += r;
            std::cout << r_parts[1] << "=" << std::fixed << std::setw(12) << std::setprecision(2) << r << ", ";
        }
        std::cout << "suma=" << std::fixed << std::setw(12) << std::setprecision(2) << sum << std::endl;
    }
    std::cout << std::endl << std::flush;


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

        std::string t = Utils::join(row_i, ' ');
        response.push_back(t);
        std::cout << t << std::endl;
    }
    return response;
}

