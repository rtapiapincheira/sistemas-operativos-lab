#include <Pipe.h>

Pipe::Pipe() :
    m_fdReader(-1),
    m_fdWriter(-1),
    m_bufferStart(0),
    m_bufferLength(0),
    m_lastError("")
{
}

Pipe::~Pipe() {
    // No-op since there's no allocation operations.
}

bool Pipe::open() {
    m_lastError = "";
    int fd[2];
    int retCode = pipe(fd);
    if (retCode != 0) {
        m_lastError = "Error while opening the pipe (return code"
                + Utils::int2string(retCode) + ").";
        return false;
    }
    m_fdReader = fd[0];
    m_fdWriter = fd[1];
    return true;
}

bool Pipe::write(_uchar *buffer, size_t len, size_t &bytesWritten) {
    m_lastError = "";
    // TODO: chequear el return code de ::write y retornar acorde, quizas deba
    // TOOD: implementar un while(write) segun el contrato de ::write
    if ((bytesWritten = ::write(m_fdWriter, (char*)buffer, len)) != (size_t)-1) {
        return true;
    }
    m_lastError = "Could not write all the bytes to the pipe.";
    return false;
}

bool Pipe::read(_uchar *buffer, size_t len, size_t &bytesRead) {
    m_lastError = "";
    bytesRead = ::read(m_fdReader, buffer, len);
    // TODO: chequear el return code de ::read, similar al write de arriba
    if ((bytesRead = ::read(m_fdReader, (char*)buffer, len)) != (size_t)-1) {
        return true;
    }
    m_lastError = "Could not read bytes from the pipe.";
    return false;
}

/*bool Pipe::readFully(char *buffer, size_t len) {
    m_lastError = "";
    size_t r = 0;
    while(r != len) {
        size_t p = 0;
        bool state = read(buffer+r, len-r, p);
        if (!state) {
            std::string newError = "ReadFully error: " + m_lastError;
            m_lastError = newError;
            return false;
        }
        r += p;
    }
    return true;
}*/

bool Pipe::consumeBytes(_uchar *buffer, size_t len) {
    if (len <= m_bufferLength) {
        size_t i = 0;
        while(len > 0) {
            _uchar nextChar = m_readBuffer[(m_bufferStart + m_bufferLength) % BUFFER_LENGTH];
            buffer[i] = nextChar;
            i++;
            m_bufferStart = (m_bufferStart+1) % BUFFER_LENGTH;
            m_bufferLength--;
            len--;
        }
        return true;
    }
    return false;
}

bool Pipe::writeString(const std::string &s) {
    m_lastError = "";
    _ushort size = s.length();

    _uchar sizeBytes[sizeof(_ushort)];
    Utils::ushort2bytes(size, sizeBytes);

    size_t written = 0;
    if (!write((char*)&sizeBytes, 2, written)) {
        m_lastError = "Couldn't send std::string.";
        return false;
    }
    if (written != 2) {
        m_lastError = "Couldn't send std::string (error while writting size)";
        return false;
    }

    if (!write((char*)s.c_str(), size, written)) {
        m_lastError = "Couldn't send std::string.";
        return false;
    }
    if (written != size) {
        m_lastError = "Couldn't send std::string (error while writting bytes).";
        return false;
    }

    // all seems ok!
    return true;
}

bool Pipe::writeInt(int x) {
    m_lastError = "";

    _uchar intBytes[sizeof(int)];
    Utils::int2bytes(x, intBytes);

    size_t written = 0;
    if (!write((char*)intBytes, 4, written)) {
        m_lastError = "Couldn't send int.";
        return false;
    }
    if (written != 4) {
        m_lastError = "Couldn't send int (written count mismatch).";
        return false;
    }

    // All seems ok!
    return true;
}

bool Pipe::writeDouble(double d) {
    m_lastError = "";
}

bool Pipe::maybePullSomeBytes() {
    // We already have plenty of bytes on the buffer, go ahead and consume them!
    if (m_bufferLength == BUFFER_LENGTH) {
        return true;
    }
    size_t maxToRead = BUFFER_LENGTH - m_bufferLength;
    size_t auxSize = 0;
    if (!read(m_auxBuffer, maxToRead, auxSize)) {
        std::string newError = "MaybePullSomeBytes error:" + m_lastError;
        m_lastError = newError;
        return false;
    }
    // Copy m_auxBuffer into m_readBuffer.
    for (size_t i = 0; i < auxSize; i++) {
        m_readBuffer
    }
    return false;
}

bool Pipe::maybeReadString(std::string &s) {
    m_lastError = "";

    if (!maybePullSomeBytes()) {
        return false;
    }

    // At least 2 bytes that indicate the size of the incoming string
    if (m_bufferLength >= 2) {
        // Build the size of the incoming string
        _uchar sizeBytes[2] = {
            m_readBuffer[m_bufferStart],
            m_readBuffer[(m_bufferStart+1) % BUFFER_LENGTH]
        };
        _ushort stringSize = Utils::bytes2ushort(sizeBytes);
        // There's enough bytes to extract a complete string
        if (m_bufferLength >= 2+stringSize) {
            // Consume the first 2bytes, as we already have the size of the
            // incoming string
            _uchar tempBuffer[2];
            consumeBytes(tempBuffer, 2);
            // Get string byte's out of the buffer, and build a proper string
            _uchar *data = new _uchar[stringSize];
            consumeBytes(data, stringSize);
            s = std::string(data); // TODO: check if this is secure
            delete[] data;
            return true;
        }
    }

    return false;

    /*
    // Read from pipe size of the incoming std::string.
    _uchar sizeBytes[sizeof(_ushort)];
    if (!readFully(sizeBytes, sizeof(_ushort))) {
        std::string newError = "MaybeReadString error (size of std::string): "
                + m_lastError;
        m_lastError = newError;
        return false;
    }
    _ushort bytesToRead = Utils::bytes2ushort(sizeBytes);

    // Read from pipe bytes of the incoming std::string.
    _uchar *buffer = new _uchar[bytesToRead];
    if (!readFully(buffer, bytesToRead)) {
        delete[] buffer;
        std::string newError = "MaybeReadString error (bytes of std::string):"
                + m_lastError;
        m_lastError = newError;
        return false;
    }
    */

}

bool Pipe::maybeReadInt(int &x) {
    m_lastError = "";
}

bool Pipe::maybeReadDouble(double &d) {
    m_lastError = "";
}

std::string Pipe::getLastError() const {
    return m_lastError;
}
