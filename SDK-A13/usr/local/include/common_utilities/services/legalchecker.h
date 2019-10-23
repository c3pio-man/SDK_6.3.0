#ifndef LEGALCHECKER_H
#define LEGALCHECKER_H

#define LEGAL_UI_APP "/ebrmain/bin/legalui.app"

class LegalChecker
{
public:
    LegalChecker();

    enum ServiceType {
        All,
        Cloud,
        BookStore,
        ReadRate,
        DeviceSetupWizard,
        Send2PB,
    };

    static bool isLegalAccepted(ServiceType t = ServiceType::All);
    static void runLegalUI();
};

#endif // LEGALCHECKER_H
