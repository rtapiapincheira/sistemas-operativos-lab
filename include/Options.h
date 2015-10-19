#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <string>
#include <vector>

#define MIN_FUND 'A'
#define MAX_FUND 'E'

/**
 * @brief The Options class handles the logic for parsing arguments from the
 * command line arguments supplied to this executable. It relays on the getopt
 * unix function to extract those params.
 */
class Options {
private:
    /**
     * @brief m_argc See the assign method.
     */
    int m_argc;
    /**
     * @brief m_argc See the assign method.
     */
    char **m_argv;

    /**
     * @brief m_hasFixedRent See the getter.
     */
    bool m_hasFixedRent;
    /**
     * @brief m_hasFixedRent See the getter.
     */
    bool m_hasVariableRent;
    /**
     * @brief m_hasFixedRent See the getter.
     */
    std::vector<char> m_fundTypes;
    /**
     * @brief m_hasFixedRent See the getter.
     */
    std::string m_fileName;

public:
    /**
     * @brief Options Constructor for this class, no-op.
     */
    Options();

    /**
     * @brief assign Sets the initial parameters for this class, in particular
     * the contents which will be analyzed in order to extract the execution
     * parameters of this application.
     * @param argc Number of arguments.
     * @param argv Actual char* NULL-terminated c-string arguments.
     */
    void assign(int argc, char **argv);

    /**
     * @brief ~Options Deallocates resources for this instance (no-op, since
     * we're not using dynamic memory.
     */
    ~Options();

    /**
     * @brief parse Parses the command line arguments (argc and argv) and tells
     * whether the parameters were ok or not.
     * @return true if the parsing was ok (all required arguments were specified
     * and with proper values) or false if not.
     */
    bool parse();

    /**
     * @brief printUsage Prints the command line invocations for this
     * application via stdout.
     */
    void printUsage();

    /**
     * @brief printArgs Prints the command line arguments passed to this
     * application via stdout.
     */
    void printArgs();

public:
    /**
     * @return  Whether should process RF or not.
     */
    bool hasFixedRent() const;

    /**
     * @return Whether should process RV or not.
     */
    bool hasVariableRent() const;

    /**
     * @return Vector with every fund to be processed (tipically 1 or 5
     * elements).
     */
    std::vector<char> getFundTypes() const;

    /**
     * @return The name of the file to process.
     */
    std::string getFileName() const;
};

#endif // _OPTIONS_H
