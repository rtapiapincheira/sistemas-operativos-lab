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

bool Pipe::closeAll() {
    return closeReader() && closeWriter();
}
bool Pipe::closeReader() {
    return (close(m_fdReader) == 0);
}
bool Pipe::closeWriter() {
    return (close(m_fdWriter) == 0);
}

bool Pipe::write(_uchar *buffer, size_t len, size_t &bytesWritten) {
    m_lastError = "";
    ssize_t st = ::write(m_fdWriter, (char*)buffer, len);
    if (st >= 0) {
        bytesWritten = (size_t)st;
        return true;
    }
    m_lastError = "Could not write all the bytes to the pipe.";
    return false;
}

bool Pipe::read(_uchar *buffer, size_t len, size_t &bytesRead) {
    m_lastError = "";
    ssize_t st = ::read(m_fdReader, (char*)buffer, len);
    if (st >= 0) {
        bytesRead = (size_t)st;
        return true;
    }
    m_lastError = "Could not read bytes from the pipe.";
    return false;
}

bool Pipe::consumeBytes(_uchar *buffer, size_t len) {
    if (len <= m_bufferLength) {
        size_t i = 0;
        while(i < len) {
            char nextChar = m_readBuffer[(m_bufferStart + i) % BUFFER_LENGTH];
            buffer[i] = nextChar;
            i++;

        }
        m_bufferStart = (m_bufferStart + len) % BUFFER_LENGTH;
        m_bufferLength -= len;
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
    if (!write(sizeBytes, 2, written)) {
        m_lastError = "Couldn't send std::string.";
        return false;
    }
    if (written != 2) {
        m_lastError = "Couldn't send std::string (error while writting size)";
        return false;
    }

    if (!write((_uchar*)s.c_str(), size, written)) {
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

bool Pipe::maybePullSomeBytes() {
    // We already have plenty of bytes on the buffer, go ahead and consume them!

    if (m_bufferLength == BUFFER_LENGTH) {
        return true;
    }
    // Read at most, as many bytes I have room for.
    size_t maxToRead = BUFFER_LENGTH - m_bufferLength;

    size_t auxSize = 0;
    if (!read(m_auxBuffer, maxToRead, auxSize)) {
        std::string newError = "MaybePullSomeBytes error:" + m_lastError;
        m_lastError = newError;
        return false;
    }

    // Copy m_auxBuffer into m_readBuffer.
    for (size_t i = 0; i < auxSize; i++) {
        // Append at the end of the current m_readBuffer.
        m_readBuffer[(m_bufferStart + m_bufferLength) % BUFFER_LENGTH]
                = m_auxBuffer[i];
        m_bufferLength++;
    }
    return true;
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
            _uchar *data = new _uchar[stringSize+1];
            consumeBytes(data, stringSize);

            // Since the std::string constructor requires a c-style string, we
            // need to make sure it ends with a NULL character.
            data[stringSize] = 0;

            s = std::string((const char*)data, stringSize);
            delete[] data;
            return true;
        }
    }
    return false;
}

std::string Pipe::getLastError() const {
    return m_lastError;
}
