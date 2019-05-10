#ifndef SERIALIZATION_UTILS_H
#define SERIALIZATION_UTILS_H

#include <iostream>
#include "stdint.h"
#include <vector>
#include <string>
#include <unistd.h>

namespace pocketbook
{
namespace utilities
{

class SerializationUtils
{
    SerializationUtils();
public:
    static int SerializeUInt16(uint16_t value, std::ostream& stream);
    static int DeserializeUInt16(std::istream& stream, uint16_t* value);

    static int SerializeInt16(int16_t value, std::ostream& stream);
    static int DeserializeInt16(std::istream& stream, int16_t* value);

    static int SerializeInt32(int32_t value, std::ostream& stream);
    static int DeserializeInt32(std::istream& stream, int32_t* value);

    static int SerializeInt64(int64_t value, std::ostream& stream);
    static int DeserializeInt64(std::istream& stream, int64_t* value);

    static int SerializePointer(void * value, std::ostream& stream);
    static int DeserializePointer(std::istream& stream, void ** value);

    static int SerializeChar(char value, std::ostream& stream);
    static int DeserializeChar(std::istream& stream, char* value);

    static int SerializeUChar(unsigned char value, std::ostream& stream);
    static int DeserializeUChar(std::istream& stream, unsigned char* value);

    static int SerializeTimestamp(int64_t timestamp, std::ostream& stream);
    static int DeserializeTimestamp(std::istream& stream, int64_t* timestamp);

    static int SerializeString(const std::string& str, std::ostream& stream);
    static int DeserializeString(std::istream& stream, std::string* str);

    static int SerializeStringVec(const std::vector<std::string>& vec, std::ostream& stream);
    static int DeserializeStringVec(std::istream& stream, std::vector<std::string>* vec);

    static int SerializeLanguage(const std::string& str, std::ostream& stream);
    static int DeserializeLanguage(std::istream& stream, std::string *str);

    static std::string ToHex(const std::string& val);
    static std::string ToHex(const std::vector<unsigned char>& val);
    static std::string FromHex(const std::string& val);

    static std::string ToBase64(const std::string &data);
    static std::string ToBase64(const std::vector<unsigned char> & data);
    static void ToBase64(const std::vector<unsigned char> & data, std::string * base64String);

    static std::vector<unsigned char> FromBase64(const std::string & str);
    static void FromBase64(const std::string & str, std::vector<unsigned char> * data);

    static void ltrim(std::string& str);
    static void rtrim(std::string& str);
    static void trim(std::string& str);


};

} // namespace usage_stat
} // namespace pocketbook

#endif // SERIALIZATION_UTILS_H
