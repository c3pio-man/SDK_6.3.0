#ifndef EXPATPARSER_H
#define EXPATPARSER_H

#include <expat.h>
#include <vector>

namespace pocketbook {
namespace utilities {

class IParser;

class ExpatParser {
public:
    ExpatParser(IParser* instance);
    ~ExpatParser();

    void init(const char* encoding="utf-8");

    bool parse(const std::vector<char>& xml){
        return parse(xml.data(), xml.size());
    }

    bool parse(const char* xml, size_t len);

private:
    XML_Parser expat;
    IParser* parser;

    static void fStartElementHandler(void *userData, const char *name, const char **attributes);
    static void fEndElementHandler(void *userData, const char *name);
    static void fCharacterDataHandler(void *userData, const char *text, int len);

    static   int fUnknownEncodingHandler(void*, const XML_Char *name, XML_Encoding *encoding);
};

} // namespace utilities
} // namespace pocketbook

#endif // EXPATPARSER_H
