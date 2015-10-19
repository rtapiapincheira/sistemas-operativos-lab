#include <GrandChildProcess.h>

GrandChildProcess::GrandChildProcess() {
    m_level = 2;
    m_name = "GrandChildProcess";

    m_allowedCodes_rv.push_back("ACC");
    m_allowedCodes_rv.push_back("ACE");
    m_allowedCodes_rv.push_back("AEE");
    m_allowedCodes_rv.push_back("CFID");
    m_allowedCodes_rv.push_back("CFMV");
    m_allowedCodes_rv.push_back("CFMD");
    m_allowedCodes_rv.push_back("CMED");
    m_allowedCodes_rv.push_back("CMEV");
    m_allowedCodes_rv.push_back("ETFA");
    m_allowedCodes_rv.push_back("ETFB");
    m_allowedCodes_rv.push_back("ETFC");

    m_allowedCodes_rf.push_back("DPF");
    m_allowedCodes_rf.push_back("ECO");
}

void GrandChildProcess::assign(const Pipe &pipe, const std::string &fileName,
        char fundType, bool isFixedRent) {
    Process::assign(pipe);
    m_fileName = fileName;
    m_fundType = fundType;
    m_isFixedRent = isFixedRent;
}

GrandChildProcess::~GrandChildProcess() {
    // No-op.
}

int GrandChildProcess::execute() {

    printStarted();

    std::ifstream in;
    in.open(m_fileName.c_str());
    if (!in.good()) {
        return 0;
    }

    FileRecord fr;

    // TODO: se hace la suma de las valorizaciones aqui o en ChildProcess?

    double totalSum = 0.0;

    std::string line;
    while(getline(in, line)) {
        //cout << "line:" << line << endl;
        _vstring parts = Utils::split(line, ';');
        if (validLine(parts, fr)) {
            if (fr.idTipoFondo == m_fundType) {
                // Process as RF
                if (m_isFixedRent) {
                    // check if idTipoInstrumentoFinanciero matches with the
                    // required rentType
                    if (fr.m_rentType == "RF") {
                        totalSum += (
                            fr.cantidad / (1.0 + (fr.tasa / 360.0) * fr.diasAlVencimiento / 100.0)
                        );
                    }
                }
                // Process as RV
                else {
                    // check if idTipoInstrumentoFinanciero matches with the
                    // required rentType
                    if (fr.m_rentType == "RV") {
                        totalSum += (fr.cantidad * fr.precio);
                    }
                }
            }
            // What-ever.
        }
    }

    in.close();

    m_parentPipe.writeString(
        m_fundType +
        std::string(",") + (m_isFixedRent ? "RF" : "RV") +
        std::string(",") + Utils::double2string(totalSum)
    );

    m_parentPipe.closeAll();

    printFinished();

    return 0;
}

//#define PRINT_VALIDATION
bool GrandChildProcess::validLine(const _vstring &parts, FileRecord &fr) {
    if (parts.size() < 6) {
        // Too few elements.
        return false;
    }

    // First value should contain only 1 char.
    std::string firstValue = Utils::trim(Utils::string2upper(parts[0]));
    if (firstValue.length() > 1) {
#ifdef PRINT_VALIDATION
        std::cout << "First value of length bigger than 1: \"" << firstValue << "\"" << std::endl;
#endif
        return false;
    }
    char type = firstValue[0];

    if (!(MIN_FUND<=type && type<=MAX_FUND)) {
        // Fund type out of range.
#ifdef PRINT_VALIDATION
        std::cout << "First value out of range: \"" << firstValue << "\"" << std::endl;
#endif
        return false;
    }
    fr.idTipoFondo = type;

    // Second value it's only a name, check is not empty.
    std::string secondValue = Utils::trim(parts[1]);
    if (secondValue.empty()) {
#ifdef PRINT_VALIDATION
        std::cout << "Second value empty: \"" << secondValue << "\"" << std::endl;
#endif
        return false;
    }
    fr.idInstrumentoFinanciero = secondValue;

    // Third value is the code, restricted to those in the task statement.
    _vstring::iterator rv_begin = m_allowedCodes_rv.begin();
    _vstring::iterator rv_end = m_allowedCodes_rv.end();

    _vstring::iterator rf_begin = m_allowedCodes_rf.begin();
    _vstring::iterator rf_end = m_allowedCodes_rf.end();

    std::string thirdValue = Utils::trim(Utils::string2upper(parts[2]));
    if (std::find(rv_begin, rv_end, thirdValue) != rv_end) {
        fr.m_rentType = "RV";
    } else if (std::find(rf_begin, rf_end, thirdValue) != rf_end) {
        fr.m_rentType = "RF";
    } else {
#ifdef PRINT_VALIDATION
        std::cout << "Third value nor RF nor RV: \"" << thirdValue << "\"" << std::endl;
#endif
        // Code not RF nor RV, unknown.
        return false;
    }
    fr.idTipoInstrumentoFinanciero = thirdValue;

    bool isOk;

    // Fourth value, valid double.
    isOk = false;
    std::string fourtValue = Utils::trim(parts[3]);
    fr.cantidad = Utils::string2double(fourtValue, &isOk);
    if (!isOk) {
#ifdef PRINT_VALIDATION
        std::cout << "Fourth value invalid double: \"" << fourtValue << "\"" << std::endl;
#endif
        // Invalid double.
        return false;
    }

    // Fifth value, valid double.
    isOk = false;
    std::string fifthValue = Utils::trim(parts[4]);
    fr.tasa = Utils::string2double(fifthValue, &isOk);
    if (!isOk) {
#ifdef PRINT_VALIDATION
        std::cout << "Fifth value invalid double: \"" << fifthValue << "\"" << std::endl;
#endif
        // Invalid double.
        return false;
    }

    // Sixth value, valid double.
    isOk = false;
    std::string sixthValue = Utils::trim(parts[5]);
    fr.precio = Utils::string2double(sixthValue, &isOk);
    if (!isOk) {
#ifdef PRINT_VALIDATION
        std::cout << "Sixth value invalid double: \"" << sixthValue << "\"" << std::endl;
#endif
        // Invalid double.
        return false;
    }

    // Seventh value, valid int.
    isOk = false;
    std::string seventhValue = Utils::trim(parts[6]);
    fr.diasAlVencimiento = Utils::string2int(seventhValue, &isOk);
    if (!isOk) {
#ifdef PRINT_VALIDATION
        std::cout << "Seventh value invalid double: \"" << seventhValue << "\"" << std::endl;
#endif
        // Invalid int.
        return false;
    }

    return true;
}
