#ifndef _GRAND_CHILD_PROCESS_H
#define _GRAND_CHILD_PROCESS_H

#include <string>
#include <Process.h>
#include <GrandChildProcess.h>

/**
 * @brief The FileRecord class represents a line of the file being processed.
 * With every part of this line already parsed and put into proper variables.
 */
class FileRecord {
public:
    char idTipoFondo;
    std::string idInstrumentoFinanciero;
    std::string idTipoInstrumentoFinanciero;
    double cantidad;
    double tasa;
    double precio;
    int diasAlVencimiento;

    /**
     * @brief m_rentType Tells wheather idTipoInstrumentoFinanciero belongs to
     * a RF or RV rent type (since idTipoInstrumentoFinanciero can only take
     * values from subsets depending of the rent type). Only values "RF" or
     * "RV".
     */
    std::string m_rentType;
};

/**
 * @brief The GrandChildProcess class defines the logic to be executed on a
 * third level process architecture (level=2). This class should have as its
 * parent the ChildProcess(level=1) process and will not spawn any more
 * sub-processes.
 *
 * All the Process handling methods are inherited from Process, refer to that
 * class for the specifics.
 *
 * In particular, this process will perform the actual business logic given the
 * options this Process is built with.
 */
class GrandChildProcess : public Process {
private:
    std::string m_fileName;
    /**
     * @brief m_fundType Fund type of the records of the file to search for.
     * Tipically one of 'A', 'B', 'C', 'D' or 'E'.
     */
    char m_fundType;

    /**
     * @brief m_isFixedRent Tells whether the processing should be done for
     * RF(true) or RV(false).
     */
    bool m_isFixedRent;

    /**
     * @brief m_allowedCodes_rv Allowed values for idTipoInstrumentoFinanciero
     * when the processing is of the type RV
     */
    _vstring m_allowedCodes_rv;

    /**
     * @brief m_allowedCodes_rf Allowed values for idTipoInstrumentoFinanciero
     * when the processing is of the type RF.
     */
    _vstring m_allowedCodes_rf;

public:
    /**
     * @brief GrandChildProcess Constructor of this class, initializes some
     * values.
     */
    GrandChildProcess();

    /**
     * @brief assign Assigns initial values for the variables of this class
     * (kind of constructor method).
     * @param pipe Pipe to communicate results to this GrandChildProcess parent
     * process.
     * @param fileName Name of the file to be processed.
     * @param fundType Type of funds this process will search for.
     * @param isFixedRent Whether perform RV or RV logic.
     */
    void assign(const Pipe &pipe, const std::string &fileName, char fundType,
            bool isFixedRent);
    /**
     * @brief ~GrandChildProcess Deallocates resources for this instance (no-op,
     * since we're not using dynamic memory).
     */
    virtual ~GrandChildProcess();

    /**
     * @brief execute Contains the logic for opening a file and processing that
     * file.
     * @return 0 if the processing was ok, other value if not (by contract).
     */
    virtual int execute();

    /**
     * @brief validLine Tells whether the line is valid for processing, and
     * additionally extracts the values to perform the business logic, only if
     * the line was valid.
     * @param parts Already split line, for extracting each of the values for
     * building the FileRecord.
     * @param fr Reference to a FileRecord variable, where the values are going
     * to be stored, if the validation process is ok.
     * @return true if the FileRecord was populated ok (i.e. the line was valid)
     * or false if not (in which case fr will contain garbage).
     */
    bool validLine(const _vstring &parts, FileRecord &fr);
};

#endif // _GRAND_CHILD_PROCESS_H
