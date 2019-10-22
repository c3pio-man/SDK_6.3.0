#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include "ttspm_global.h"

#include <QMutex>
#include <QObject>
#include <QThread>
#include <QWaitCondition>

namespace pocketbook {
namespace ttspm {

class TTSPMSHARED_EXPORT PackageManager : public QObject
{
    Q_OBJECT
public:

    enum PackageInstallStatus {
        Successfull,
        ErrorNoSpace,
        Error,
        None,
    };

    explicit PackageManager(QObject *parent = nullptr);

signals:
    void progressChanged(double percent);
    void installFinished(int status);
    void uninstallFinished(int status);

public slots:
    void install(QString fullPackagePath);
    void uninstall(QString packageName);

private:
};

}
}

#endif // PACKAGEMANAGER_H
