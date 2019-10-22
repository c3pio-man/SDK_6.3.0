#ifndef TTSPM_H
#define TTSPM_H

#include "ttspm_global.h"

#include "ttspmhelper.h"

#include <vector>

#include <QJsonDocument>
#include <QUrl>

namespace pocketbook {
namespace ttspm {

#define PACKAGE_PROCESSING_LOCK_FILE "/tmp/tts_package_processing_lock"

class TTSPMSHARED_EXPORT TTSPM
{

public:
    TTSPM();
    static void updateLatestTTSVoicesListFromUrl();
    static std::vector<TTSPackage> getLatestTTSVoicesFromUrl();
    static std::vector<TTSPackage> getLatestTTSVoicesFromFile();
    static std::vector<TTSPackage> getLatestTTSVoicesFromJson(QJsonDocument ttsInfoJsonDocument);
    static std::vector<TTSPackage> getInstalledTTSVoices();
    static std::vector<QString> getInstalledTTSVoicesNames();
    static std::vector<TTSPackage> getUpdatedTTSVoices();
    static std::vector<TTSPackageGroup> getTTSVoicesGroups();
    static std::vector<TTSPackage> getTTSVoicesForGroup(QString group);
    static bool isPackageInstalledByName(QString ttsPackageName);
    static QString getPackageInstalledRealNameByName(QString ttsPackageName);
    static bool isPackageProcessing();
    static QString getPackageProcessingErrorString(TTSPackageProcessingError error);
};

}
}

#endif // TTSPM_H
