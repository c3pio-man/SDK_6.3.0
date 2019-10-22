#ifndef PACKAGES_H
#define PACKAGES_H

#include "ttspm_global.h"

#include <QString>

namespace pocketbook {
namespace ttspm {

enum TTSPackageProcessingError {
    PPE_None,
    PPE_DownloadNoSpace,
    PPE_DownloadError,
    PPE_InstallNoSpace,
    PPE_InstallError,
    PPE_UserAbort,
};

struct TTSPackage {
    QString packageId;
    QString packageUrl;
    QString packageFileName;
    QString packageVersion;
    qlonglong packageSize;
    QString povider;
    QString group;
    QString fullName;
    QString name;
    QString version;
    QString locale;
    QString lang;
    QString gender;
    QString sampleUrl;
};

struct TTSPackageGroup {
    QString name;
    QString localeName;

};

bool operator==(const TTSPackageGroup& left, const TTSPackageGroup& right);
bool operator<(const TTSPackageGroup& left, const TTSPackageGroup& right);

QDebug operator<<(QDebug debug, const TTSPackage &ttsPackage);
QDebug operator<<(QDebug debug, const TTSPackageGroup &ttsPackageGroup);

}
}

#endif // PACKAGES_H
