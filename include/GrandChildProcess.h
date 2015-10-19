#ifndef _GRAND_CHILD_PROCESS_H
#define _GRAND_CHILD_PROCESS_H

#include <string>
#include <Process.h>
#include <GrandChildProcess.h>

class FileRecord {
public:
    char idTipoFondo;
    std::string idInstrumentoFinanciero;
    std::string idTipoInstrumentoFinanciero;
    double cantidad;
    double tasa;
    double precio;
    int diasAlVencimiento;

    std::string m_rentType;
};

class GrandChildProcess : public Process {
private:
    std::string m_fileName;
    char m_fundType;
    bool m_isFixedRent;

    _vstring m_allowedCodes_rv;
    _vstring m_allowedCodes_rf;

public:
    GrandChildProcess();
    void assign(const Pipe &pipe, const std::string &fileName, char fundType,
            bool isFixedRent);
    virtual ~GrandChildProcess();
    virtual int execute();

    bool validLine(const _vstring &parts, FileRecord &fr);
};

#endif // _GRAND_CHILD_PROCESS_H
