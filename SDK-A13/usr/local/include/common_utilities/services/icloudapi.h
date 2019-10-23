#ifndef ICLOUDAPI_H
#define ICLOUDAPI_H
#include <map>
#include <string>

namespace pocketbook {
namespace utilities {
class ICloudApi
{
public:
    enum AutoAuthParams {
        kLogin,
        kPassword,
        kProviderName,
        kProviderAlias,
        kShopId,
    };

    virtual bool IsLoggedIn() = 0;
    virtual void setParamsForAutoAuth(const std::map<AutoAuthParams, std::string>& params) = 0;
    virtual std::map<AutoAuthParams, std::string> getParamsForAutoAuth() = 0;
    virtual int startAutoAuth(bool need_adobe_activation = true) = 0;
    virtual int logOut() = 0;
    virtual bool havePBCloud() = 0;
    virtual bool needAutoRegistrationToCloud() = 0;
    virtual bool IsSyncing() = 0;
    virtual int Syncronize() = 0;
    virtual int deleteFromCloud(const char* path) = 0;

    enum SyncError {
        kUnknown,
        kNoSpace,
        kLegalTermsAreNotAccepted,
    };

    virtual SyncError getSyncError() = 0;
    virtual ~ICloudApi() {};

};

ICloudApi* createCloudApi();
}}
#endif // ICLOUDAPI_H
