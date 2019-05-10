#ifndef IPARSER_H
#define IPARSER_H

#include <string>
#include <unordered_map>

namespace pocketbook {
namespace utilities {

class IParser
{
public:
    virtual ~IParser(){}

    virtual void reset() =0;
    virtual void startElement(const std::string& name, const std::unordered_map<std::string, std::string>& attributes) =0;
    virtual void endElement(const std::string& name) =0;
    virtual void data(const std::string& part) =0;
};

} // namespace utilities
} // namespace pocketbook

#endif // IPARSER_H
