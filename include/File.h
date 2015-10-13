#ifndef _FILE_H
#define _FILE_H

#include <fstream>
#include <string>

class File {
private:
    std::string m_fileName;
    std::ifstream *m_stream;
    std::string m_lastError;

public:
    File(const std::string &fileName);
    ~File();

    bool open();
    bool close();

    bool readLine(std::string &s);
    bool hasNextLine();
    std::string getLastError() const;
};

#endif // _FILE_H
