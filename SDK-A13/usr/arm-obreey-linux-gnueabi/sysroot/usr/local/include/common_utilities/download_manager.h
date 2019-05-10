#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H


#include "common_utilities/downloader.h"
#include "common_utilities/message_queue.h"
#include "memory"
#include "pthread.h"

namespace pocketbook
{
namespace utilities
{

using std::shared_ptr;

class DownloadManager: IDownloaderCallback
{
    class DownloadCompleteCallback
    {
    public:
        virtual void Complete(int rc, FileToSync& item) = 0;
        virtual ~DownloadCompleteCallback(){}
    };

    template<typename F> class DownloadCompleteCallbackWrapper: public DownloadCompleteCallback
    {
        F f_;
    public:
        DownloadCompleteCallbackWrapper(const F& f):f_(f){}
        virtual void Complete(int rc, FileToSync& item)
        {
            f_(rc, item);
        }
    };

public:    
    DownloadManager();
    ~DownloadManager();

    int Init(const std::string& download_folder);
    template <typename F> int AddItem(const FileToSync& item, const F& complete_callback)
    {
        shared_ptr<DownloadCompleteCallback> callback(new DownloadCompleteCallbackWrapper<F>(complete_callback));
        return AddItemInt(item, callback);
    }

private:
    virtual void NotifyDownloaderStatus(void* user_data);
    int AddItemInt(const FileToSync& item, shared_ptr<DownloadCompleteCallback> complete_callback);
    int DownloadNextItem();

    struct ManagedItem {
        FileToSync file_;
        shared_ptr<DownloadCompleteCallback> callback_;
        ManagedItem(const FileToSync& file, shared_ptr<DownloadCompleteCallback> complete_callback):
            file_(file), callback_(complete_callback){}
    };
    void FinalizeItem(ManagedItem* item, int rc);

    Downloader downloader_;
    std::string download_folder_;
    MessageQueue<ManagedItem*> files_to_download_;
    ManagedItem* current_item_;
    pthread_mutex_t mutex_;
};

} // namespace utilities
} // namespace pocketbook

#endif // DOWNLOAD_MANAGER_H
