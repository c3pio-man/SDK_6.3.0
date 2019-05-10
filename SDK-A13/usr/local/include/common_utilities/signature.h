#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <string>
#include "http_request.h"
namespace pocketbook
{
namespace utilities
{

class Signature
{
public:
    static std::string CreateSignature(const HttpRequest& request, const std::string& pwd_hash, const std::string& client_secret);
    static std::string GenerateNonce();
    static std::string ToHex(const std::string& val);
};

} // namespace utilities
} // namespace pocketbook


#endif // SIGNATURE_H
