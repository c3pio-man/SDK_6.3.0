#ifndef BOOKSTORE_AUTH_CONFIG_H
#define BOOKSTORE_AUTH_CONFIG_H

#include <string>
#include <map>

namespace pocketbook {
namespace utilities {

class BookStoreAuthConfigs {
public:
    static std::string GetLoginKey();
    static std::string GetPasswordKey();
    static std::string GetTokenKey();
    static std::string GetConfigFilename();
    static bool isLoggedIn();
    static void runBookStoreLogin(bool need_open_store_after);
    static void logOut();
    struct PartnerStoreInfo {
        std::string config_filename_;
        std::string login_key_;
        std::string password_key_;
        std::string token_key_;
    };

    enum class AuthScenario {
        BookStoreRunFromDesktop,
        BookStoreAuthFromSettings,
        BookStoreRegisterFromSettings,
        CloudLogin,
        LoginFromStore,
    };

    enum class AuthStep {
        StartStep,
        LoginToStore,
        RegisterToStore,
        SetCloudParams,
        LoginToCloud,
        RunStore,
        ShowDialog,
        Finish,
    };

    enum class StepResult {
        Success,
        Skip,
        Back,
    };

    struct AuthState {
        AuthScenario scenario_;
        AuthStep step_;
    };

    enum class FutherAction {
        Wait,
        Close,
        Finish,
        ShowDialog,
    };
    static void runScenarioStarter(AuthScenario scenario);

    AuthStep getCurrentStep() { return state_.step_; }
    AuthScenario getCurrentScenario() { return state_.scenario_; }
    void parseState(const std::string& state);
    FutherAction Proceed(AuthStep step, StepResult result);
    FutherAction startScenario(AuthScenario scenario);

private:
    FutherAction BookstoreFormDesktop(AuthStep step, StepResult result);
    FutherAction KnvUmbreitBookstoreFromDesktop(AuthStep step, StepResult result);
    FutherAction KnvUmbreitBookstoreLoginFromSettings(AuthStep step, StepResult result);
    FutherAction KnvUmbreitCloudLogin(AuthStep step, StepResult result);
    FutherAction BuchmediaFromDesktop(AuthStep step, StepResult result);
    FutherAction BuchmediaLoginFromSettings(AuthStep step, StepResult result);
    FutherAction BuchmediaCloudLogin(AuthStep step, StepResult result);
    FutherAction LoginFromStore(AuthStep step, StepResult result);
    void runStore();
    void runStoreInLoginMode();
    void runStoreInRegisterMode();

    void runCloudAuth();
    void runStoreInSetCloudParamsMode();
    void runStoreNativeLogin();
    void runStoreNativeRegister();

    std::string stateToString();
    static PartnerStoreInfo getCurrentPartnerStoreInfo();

    AuthState state_;
};

}}
#endif // BOOKSTORE_AUTH_CONFIG_H
