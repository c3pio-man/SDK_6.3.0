#ifndef USER_AGENT_CONFIG_H
#define USER_AGENT_CONFIG_H

#include <string>
#include <map>

namespace pocketbook {
namespace utilities {


class UserAgentConfig {
public:
    UserAgentConfig(const std::string& qt_version, const std::string& web_engine_version);

    std::string userAgentForHost(const std::string& host) const;

private:
    std::string SubstituteTemplateParameters(const std::string& templ) const;
    void initDefaultUserAgent();
    std::string getTemplateForHost(const std::string& host) const;

    std::string languageToLayout(const std::string & language) const;

    std::string getConfigInterfaceLanguage() const;

    std::string browserLangString() const;

    std::string ResolutionScreenString() const;

    std::string GetModelNumber() const;

    void fillSubstituteParams();

    void AddUserAgentConfig(const std::string& name, const std::string& value);

    void loadUserAgentConfig();


    std::map<std::string, std::string> substitute_params_;
    std::string qt_version_;
    std::map<std::string, std::string> user_agents_;
    std::string default_user_agent_;
    std::string web_engine_version_;
};

}
}
#endif // USER_AGENT_CONFIG_H
