#ifndef RESOURCE_DOWNLOADER_H
#define RESOURCE_DOWNLOADER_H

#include "common_utilities/downloader.h"
#include "common_utilities/message_queue.h"
#include "common_utilities/working_thread.h"
#include <functional>
#include <memory>
#include "pthread.h"
#include <map>

namespace pocketbook
{
namespace utilities
{

class SyncDownloader: public  pocketbook::utilities::IDownloaderCallback
{
public:
    SyncDownloader(const std::string& sync_folder);
    int Download(const std::string& url, const std::string& filename);

    int GetProgress(bool* is_running, std::string* url, int* total, int* downloaded);
    void CancelCurrent();
    void SetSyncFolder(const std::string& folder);
    std::string GetSyncFolder();
    std::string GetFinalFileName() const { return final_filename_; }
    pocketbook::utilities::Downloader::StatusInfo GetStatusInfo() const { return last_status_info_; }
    void AddHeader(const std::string& name, const std::string& value);
    void ClearHeaders();
private:
    virtual void NotifyDownloaderStatus(void* user_data);

    pocketbook::utilities::Downloader::Status download_status_;
    pocketbook::utilities::Downloader downloader_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    std::string sync_folder_;
    pocketbook::utilities::Downloader::StatusInfo last_status_info_;
    std::string final_filename_;
};

}
}
#endif // RESOURCE_DOWNLOADER_H
