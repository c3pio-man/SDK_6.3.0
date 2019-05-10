#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>
#include <string>
#include <map>
#include <vector>

namespace pocketbook {
namespace utilities {

/**
 * @brief Class for parse and check input parameters. Very helpful when use openbook
 * http://www.math.uni-hamburg.de/doc/java/tutorial/essential/attributes/_posix.html
 * Sample: parameters {"-o", "-fd", "-ptest", "unknown", "-k test2"}
 * IsOptPresent return true for 'o','f','d','p','t','e','s','k' chars
 * GetOptArg return "test" for 'p' option and "test2" for 'k' option
 */
 
class Parameters
{
public:
    Parameters();
    
    /**
     * @brief Set parameters for parse
     * @param argc - count of parameters
     * @param argv - array of parameters
     */
    void Set(const int argc, const char * const * argv);
    void Set(const char * parameters);
    void Set(const std::string &parameters);
    
    /**
     * @brief Return parameter for option if option and parameter are available
     * @param opt - option
     * @return parameter if available or ampty string in otherwise
     */
    std::string GetOptArg(const char opt) const;
    std::vector<std::string> GetAllOptArgs(const char opt) const;
    
    /**
     * @brief check parameters string for presence of option
     * @param opt - option to check
     * @return true if option is present ot false in otherwise
     */
    bool IsOptPresent(const char opt) const;
    
    std::multimap<char, std::string> GetAllOptions() const {return opt_list_;}
    /**
     * @brief Check for presence of an argument
     * @param arg - argument to check
     * @return true if available or false in otherwise
     */
    bool IsArgumentPresent(const std::string &arg) const;

    std::vector<std::string> GetAllArguments() const {return arguments_list_;}
private:
    void AnalizeString();
    std::multimap<char, std::string> Break2OptPair(const std::vector<std::string> &argv);
    std::vector<std::string> Break2Arguments(const std::vector<std::string> &argv);
    std::string LTrim(const std::string &str) const;
    std::string RTrim(const std::string &str) const;
    size_t FindEndEscapedParameter(const std::string &str) const;
private:
    std::vector<std::string> argv_;
    std::multimap<char, std::string> opt_list_;
    std::vector<std::string> arguments_list_;
};

} // namespace utilities
} // namespace pocketbook

#endif // PARAMETERS_H
