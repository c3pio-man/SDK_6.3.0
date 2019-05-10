#pragma once
#include <string>
#include <vector>

namespace pocketbook {
namespace utilities {

class CountryCodes
{
    struct Metadata {
        std::string fips;
        std::string alpha_2;
        std::string alpha_3;
        std::string numeric_3;
    };
    
    enum class eCode {
        kFips = 0,
        kAlpha2,
        kAlpha3,
        kNumeric3
    };
public:
    
    
    static std::string GetFipsFromAlpha2(const std::string &alpha2);
    static std::string GetFipsFromAlpha3(const std::string &alpha3);
    static std::string GetFipsFromNumeric3(const std::string &numeric3);
    
    static std::string GetAlpha2FromFips(const std::string &fips);
    static std::string GetAlpha2FromAlpha3(const std::string &alpha3);
    static std::string GetAlpha2FromNumeric3(const std::string &numeric3);
    
    static std::string GetAlpha3FromFips(const std::string &fips);
    static std::string GetAlpha3FromAlpha2(const std::string &alpha2);
    static std::string GetAlpha3FromNumeric3(const std::string &numeric3);
    
    static std::string GetNumeric3FromFips(const std::string &fips);
    static std::string GetNumeric3FromAlpha2(const std::string &alpha2);
    static std::string GetNumeric3FromAlpha3(const std::string &alpha3);
private:
    static CountryCodes * GetInstance();
    CountryCodes();
    void LoadCountryCodes();
    const std::vector<Metadata> &GetCodeList() {return code_list_;}
    std::string FindCode(eCode find_code, const std::string &code, eCode returned_code);
    std::string GetCode(eCode code, size_t pos);
private:
    std::vector<Metadata> code_list_;
};

} // namespace utilities
} // namespace pocketbook
