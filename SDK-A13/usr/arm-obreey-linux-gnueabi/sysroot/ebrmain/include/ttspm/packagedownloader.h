#ifndef PACKAGEDOWNLOADER_H
#define PACKAGEDOWNLOADER_H

#include "ttspm_global.h"

#include <QObject>
#include <QMutex>
#include <QTimer>

#include <common_utilities/downloader.h>

namespace pocketbook {
namespace ttspm {

class TTSPMSHARED_EXPORT PackageDownloader : public QObject, public pocketbook::utilities::IDownloaderCallback
{
    Q_OBJECT

public:
    enum PackageDownloadStatus {
        Successfull,
        Error,
        None,
    };

    explicit PackageDownloader(QObject *parent = nullptr);
    void NotifyDownloaderStatus(void *user_data) override;

signals:
    void progressChanged(double percents);
    void finished(int status);
    void needStopTimer();

public slots:
    int start(QString url, QString filename, QString folder);
    void abort();

private slots:
    void onTimer();
    void stopTimer();

private:
    pocketbook::utilities::Downloader downloader_;
    pocketbook::utilities::FileToSync current_file_;

    QTimer timer_;

    bool cleaned_ = true;

    QMutex mutex_;
};

}
}

#endif // PACKAGEDOWNLOADER_H
