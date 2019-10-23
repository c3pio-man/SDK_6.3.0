#ifndef SINGLEAUTH_H
#define SINGLEAUTH_H

#include <string>
#include <map>

namespace pocketbook {
namespace utilities {
class SingleAuth
{
public:
    SingleAuth();

    std::string getOAuthUrl(const std::string& provider = "");

    struct Token {
        std::string access_token_;
        std::string refresh_token_;
        time_t expired_in_ = 0;
    };

    std::map<std::string, Token> getTokens(const std::string& code);

    std::map<std::string, Token> getTokensByStoreToken(const std::string& store_token);

    std::pair<std::string, std::string> getAuthQrCode();

    int sendQrCodeAuth(const std::string& access_token, const std::string& qr_code);

    std::map<std::string, Token> getAppTokens(const std::string& access_token);
private:
    std::string client_id_;
};

}
}


#endif // SINGLEAUTH_H
