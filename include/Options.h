#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <string>
#include <vector>

#define MIN_FUND 'A'
#define MAX_FUND 'E'

class Options {
private:
    int m_argc;
    char **m_argv;

    bool m_hasFixedRent;
    bool m_hasVariableRent;
    std::vector<char> m_fundTypes;
    std::string m_fileName;

public:
    Options();
    void assign(int argc, char **argv);
    ~Options();

    bool parse();
    void printUsage();
    void printArgs();

public:
    bool hasFixedRent() const;
    bool hasVariableRent() const;
    std::vector<char> getFundTypes() const;
    std::string getFileName() const;
};

#endif // _OPTIONS_H
