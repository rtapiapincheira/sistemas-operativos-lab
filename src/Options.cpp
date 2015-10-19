#include <Options.h>

#include <iostream>
#include <getopt.h>

Options::Options() {

}

void Options::assign(int argc, char **argv) {
    m_argc = argc;
    m_argv = argv;
}
Options::~Options() {
    // No-op.
}

bool Options::parse() {

    m_fileName.clear();
    m_fundTypes.clear();
    for (char t = MIN_FUND; t <= MAX_FUND; t++) {
        m_fundTypes.push_back(t);
    }
    m_hasFixedRent = true;
    m_hasVariableRent = true;

    static struct option long_options[] = {
        {"Archivo",      required_argument, 0, 'f'},
        {"TipoDeFondo",  required_argument, 0, 't'},
        {"TipoDeActivo", required_argument, 0, 'a'},
        {0,              0,                 0, 0, }
    };

    std::string rent;
    char fund = 0;
    int long_index = 0;

    int opt = 0;
    while((opt = getopt_long(m_argc, m_argv, "f:t:a:", long_options,
            &long_index)) != -1) {
        switch(opt) {
        case 'f':
            if (!optarg) {
                return false;
            }
            // We should check if is valid file, but it's ok.
            m_fileName = std::string(optarg);
            break;
        case 't':
            fund = optarg[0];
            if (fund < MIN_FUND || MAX_FUND < fund) {
                return false;
            }
            // Override the default TipoDeFondo with anything passed as arg.
            m_fundTypes.clear();
            m_fundTypes.push_back(fund);
            break;
        case 'a':
            rent = std::string(optarg);
            if (rent != "RF" && rent != "RV") {
                return false;
            }
            // One or the other.
            m_hasFixedRent = (rent == "RF");
            m_hasVariableRent = (rent == "RV");
            break;
        default:
            return false;
        }
    }
    if (m_fileName.empty()) {
        return false;
    }

    return true;
}

void Options::printUsage() {
    std::string program = "./program";

    std::cout << std::endl;
    std::cout << "Long options:" << std::endl;
    std::cout << "Usage: " << program << " --Archivo <file> [--TipoDeFondo <A|B|C|D|E|F>] [--TipoDeActivo <RF|RV>]" << std::endl;
    std::cout << "Example 1: " << program << " --Archivo data.txt --TipoDeFondo A --TipoActivo RF" << std::endl;
    std::cout << "Example 2: " << program << " --Archivo data.txt --TipoDeFondo A" << std::endl;
    std::cout << "Example 3: " << program << " --Archivo data.txt" << std::endl;

    std::cout << std::endl;
    std::cout << "Short options:" << std::endl;
    std::cout << "Usage: " << program << " -f <file> [-t <A|B|C|D|E|F>] [-a <RF|RV>]" << std::endl;
    std::cout << "Example 4: " << program << " -f data.txt -t A -a RF" << std::endl;
    std::cout << "Example 5: " << program << " -f data.txt -t A" << std::endl;
    std::cout << "Example 5: " << program << " -f data.txt" << std::endl;

    std::cout << std::endl;
    std::cout << std::flush;
}

void Options::printArgs() {
    for (int i = 0; i < m_argc; i++) {
        if (i > 0) {
            std::cout << " ";
        }
        std::cout << m_argv[i];
    }
    std::cout << std::endl;

    std::cout << std::flush;
}

bool Options::hasFixedRent() const {
    return m_hasFixedRent;
}
bool Options::hasVariableRent() const {
    return m_hasVariableRent;
}
std::vector<char> Options::getFundTypes() const {
    return m_fundTypes;
}
std::string Options::getFileName() const {
    return m_fileName;
}
