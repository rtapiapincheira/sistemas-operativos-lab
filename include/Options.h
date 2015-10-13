#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <string>
#include <vector>

class Options {
private:
    int m_argc;
    char **m_argv;

    bool m_hasFixedRent;
    bool m_hasVariableRent;
    std::vector<char> m_fundTypes;
    std::string m_fileName;

public:
    Options(int argc, char **argv);
    ~Options();

private:
    void parse();

public:
    bool hasFixedRent() const;
    bool hasVariableRent() const;
    std::vector<char> getFundTypes() const;
    std::string getFileName() const;
};

#endif // _OPTIONS_H
