#pragma once

#include <string>
#include <vector>

namespace pocketbook {
namespace utilities {

class LanguageCodes
{
    struct Metadata {
        std::string iso639_2_b;
        std::string iso639_2_t;
        std::string iso639_1;
        std::string eng_name;
    };
    
public:
    enum class eCode {
        kISO639_2_b = 0,
        kISO639_2_t,
        kISO639_1,
        kEnglishName
    };
    static std::string FindCode(eCode find_code, const std::string &code, eCode returned_code);

private:
    static LanguageCodes * GetInstance();
    LanguageCodes();
    void LoadLanguageCodes();
    const std::vector<Metadata> &GetCodeList() {return code_list_;}
    std::string GetCode(eCode code, size_t pos);
private:
    std::vector<Metadata> code_list_;
};

} // namespace utilities
} // namespace pocketbook
