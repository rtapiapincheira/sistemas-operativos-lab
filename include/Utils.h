#ifndef _UTILS_H_
#define _UTILS_H_

#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

typedef unsigned short _ushort;
typedef unsigned char _uchar;

typedef std::vector<std::string> _vstring;
typedef std::vector<_vstring> _vvstring;

/**
 * @brief The Utils class constains useful definitions related to formatting,
 * serializing and string manipulation. It does not need an instance, since all
 * the methods are static.
 */
class Utils {
public:
    /**
     * @brief int2string Converts a signed int into a std::string.
     * @param x value to be converted.
     * @return non emty string, for the value passed as argument.
     */
    static std::string int2string(int x);

    /**
     * @brief ushort2bytes Converts an _ushort value into its 2-byte MSB
     * representation.
     * @param x Value to be converted.
     * @param buffer Where to put the 2 bytes.
     */
    static void ushort2bytes(_ushort x, _uchar *buffer);
    /**
     * @brief int2bytes Converts an int value into its n-bytes MSB
     * representation (where n = sizeof(int)).
     * @param x Value to be converted.
     * @param buffer Where to put the n bytes.
     */
    static void int2bytes(int x, _uchar *buffer);

    /**
     * @brief bytes2ushort Inverse function for the ushort2string function.
     * @param buffer Bytes to convert back into an _ushort.
     * @return Ushort value for the value converted.
     */
    static _ushort bytes2ushort(_uchar *buffer);
    /**
     * @brief bytes2int Inverse function for the int2bytes function.
     * @param buffer Bytes to convert back into an int.
     * @return Int value for the value converted.
     */
    static int bytes2int(_uchar *buffer);

    /**
     * @brief split separates an string into a vector of strings, using the
     * supplied separator.
     * @param line Line to be split.
     * @param sep Separator to perform the operation.
     * @return vector of strings, never empty (at least 1 element, "").
     */
    static _vstring split(const std::string &line, char sep = ';');
    /**
     * @brief join concatenates all the strings in the vector passed as argument
     * using the separator as glue.
     * @param v Elements to be concatenated.
     * @param sep Separator (string between consecutive concatenated elements).
     * @return string, never emtpy, for the concatenation of the elements.
     */
    static std::string join(const _vstring &v, char sep = ';');

    /**
     * @brief trim Removes elements at the beginning and end of the string,
     * useful to perform sanitizations.
     * @param s String to be trimmed (cleaned at the beginning and end).
     * @param chars Elements to remove (by default only spaces).
     * @return trimmed string, possibly empty.
     */
    static std::string trim(const std::string &s,
            const std::string &chars = "\t\r\n \b");

    /**
     * @brief string2double Converts a string with contents of a valid double,
     * into a 64-bit floating point double value. It tells whether the string
     * was a valid value or not.
     * @param s String containing the value to be converted.
     * @param ok Address of a variable that will tell wheather the conversion
     * was succesful or not. If NULL, will omit the check (not recommended).
     * @return the double value parsed from the string (*ok set to true), or
     * zero if the string was not a valid double (*ok set to false).
     */
    static double string2double(const std::string &s, bool *ok = NULL);

    /**
     * @brief string2int Converts a string with content of a valid int, into a
     * sizeof(int)-bytes integer value. It tells whether the string was a valid
     * value or not.
     * @param s String containing the value to be converted.
     * @param ok Address of a variable that will tell wheather the conversion
     * was successful or not. If NULL, will omit the check (not recommended).
     * @return the int value parsed from the string (*ok set to true), or zero
     * if the string was not a valid int (*ok set to false).
     */
    static int string2int(const std::string &s, bool *ok = NULL);

    /**
     * @brief string2upper Changes the case of the string passed, by copying the
     * argument.
     * @param s string to be converted into upper case.
     * @return copy of the string passed as argument, changed its case to upper
     * case.
     */
    static std::string string2upper(const std::string &s);

    /**
     * @brief double2string Converts a double value (64-bit floating point on
     * most machines) to its string equivalent expression (suitable to
     * concatenate to other strings).
     * @param x Value to be converted as string.
     * @return non-empty string for the value converted.
     */
    static std::string double2string(double x);
};

#endif // _UTILS_H_
