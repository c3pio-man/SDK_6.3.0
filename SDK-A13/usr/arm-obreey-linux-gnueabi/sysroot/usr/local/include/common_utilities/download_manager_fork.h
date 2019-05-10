#ifndef DOWNLOAD_MANAGER_FORK_H
#define DOWNLOAD_MANAGER_FORK_H

#include <functional>
#include <string>
#include <thread>
#include "common_utilities/message_queue.h"
#include "common_utilities/downloader.h"

namespace pocketbook {
namespace utilities {

class DownloadManagerFork
{
public:
    DownloadManagerFork();
    ~DownloadManagerFork();

    int Init(const std::string& download_folder);
    int AddItem(const FileToSync& item, std::function<void(int, FileToSync)> complete_callback);
private:

    void thread_func();
    std::string download_folder_;
    struct DownloadItem {
        FileToSync file_;
        std::function<void(int, FileToSync)> cb_;
    };

    MessageQueue<DownloadItem> files_to_download_;
    std::thread thread_;
    bool is_running_;
};

}
}

#endif // DOWNLOAD_MANAGER_FORK_H
