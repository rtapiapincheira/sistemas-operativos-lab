#ifndef _PIPE_H
#define _PIPE_H

#include <string>
#include <unistd.h>

#include <Utils.h>

#define BUFFER_LENGTH 4096

/**
 * @brief The Pipe class represents a two-way communication channel, suitable
 * to communicate two different process. Internally, is a wrapper for a plain
 * old pipe, for UNIX operating systems.
 *
 * This class provides high-level methods for sending structured data-types, and
 * does not require the caller to perform enconding operations prior to sending
 * bytes (this class handles that).
 *
 * The usage of this class comes in handy used along fork() calls, to
 * communicate parent-child processes. To do so, make sure to create this object
 * prior to the fork() method call.
 */
class Pipe {
private:
    /**
     * @brief m_fdReader File descriptor for the reading endpoint of the pipe.
     */
    int m_fdReader;

    /**
     * @brief m_fdWriter File descriptor for the writing endpoint of the pipe.
     */
    int m_fdWriter;

    /**
     * @brief m_readBuffer Internal buffer that stores partially read
     * information extracted out this pipe by calling the maybeReadX methods.
     */
    _uchar m_readBuffer[BUFFER_LENGTH];
    _uchar m_auxBuffer[BUFFER_LENGTH];

    /**
     * @brief m_bufferStart Position of the first byte of data.
     */
    size_t m_bufferStart;

    /**
     * @brief m_bufferLength Number of elements on the buffer. It can't be
     * larger than BUFFER_LENGTH.
     */
    size_t m_bufferLength;

    /**
     * @brief m_lastError Description of the last error detected by conditions
     * in this class. It may be due to failed read/write operations on the
     * subyacent pipe.
     */
    std::string m_lastError;

public:
    /**
     * @brief Pipe Constructs a new Pipe ready to be used.
     */
    Pipe();

    /**
     * @brief ~Pipe Deallocates resources allocated via constructor.
     */
    ~Pipe();

    /**
     * Opens the pipe, and allows all read/write operations on both endpoints of
     * this pipe; make sure to open it before reading/writing, or it possibly it
     * may yield garbage, beside getLastError returning non-empty.
     *
     * @brief open Opens the pipe, to make it available for reading/writing.
     * @return true on success and the pipe was open, otherwise false (
     * getLastError would return non-empty).
     */
    bool open();

private:
    /**
     * Generic write operations, it simply wraps the system write I/O function
     * call by using always the same file descriptor id contained within this
     * class (the writer endpoint of the pipe).
     *
     * @brief write Writes bytes on the writer endpoint of this class.
     * @param buffer Pointer to a memory location to be written.
     * @param len Number of bytes to write.
     * @param bytesWritten Number of bytes effectively written to the pipe.
     * @return true on success, otherwise false (getLastError would return
     * non-empty).
     */
    bool write(_uchar *buffer, size_t len, size_t &bytesWritten);

    /**
     * @brief read Reads bytes from the reader endpoint of this class.
     * @param buffer Pointer to a memory location where the bytes are to be
     * stored.
     * @param len Max size of the memory location (hard limit for the number of
     * bytes to be read).
     * @param bytesRead Number of bytes effectively read from the pipe (it's
     * possible to be zero, which doesn't mean error).
     * @return true on success, otherwise false (getLastError would return
     * non-empty).
     */
    bool read(_uchar *buffer, size_t len, size_t &bytesRead);

    bool consumeBytes(_uchar *buffer, size_t len);

public:
    /**
     * Serializes a std::string object and writes it into this pipe, making it
     * available to be read at the other endpoint via maybeReadString.
     *
     * @brief writeString Writes a std::string object into the pipe.
     * @param s String to be sent.
     * @return true on success, otherwise false (getLastError would return
     * non-empty).
     */
    bool writeString(const std::string &s);

    bool maybePullSomeBytes();

    /**
     * Deserializes a std::string object out of this pipe. The value should've
     * been written in the other endpoint of this pipe using the writeString
     * method.
     *
     * @brief maybeReadString Reads a std::string if it's possible to do so.
     * @param s std::string reference where the value is to be stored.
     * @return true on success (the value could be read) or false if it couldn't
     * or there was an error, in which case getLastError will return non-empty.
     */
    bool maybeReadString(std::string &s);

    /**
     * @brief getLastError Returns the error description of a just failed
     * operation.
     * @return non-empty string value whenever the pior operation failed,
     * stating the error description for the just failed operation. If no error
     * occurred (or none was detected by conditions in this implementation), an
     * empty std::string value will be returned instead.
     */
    std::string getLastError() const;
};

#endif // _PIPE_H
