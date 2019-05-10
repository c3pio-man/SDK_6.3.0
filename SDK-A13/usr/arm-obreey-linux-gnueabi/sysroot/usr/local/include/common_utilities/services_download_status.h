#ifndef SERVICES_DOWNLOAD_STATUS_H
#define SERVICES_DOWNLOAD_STATUS_H

#include <string>
#include <semaphore.h>
#include <vector>
#include <map>

namespace pocketbook
{
namespace utilities
{

class DownloadStatus
{
public:
    enum Error {
        kNoError,
        kErrNoSpace,
        kErrAuthExpired,
        kErrNetworkFailed,
        kErrServerResponce,
        kErrAbortedByUser,
        kErrInvalidFolder,
        kErrUnknown,
        kErrRestrictedContent,
        kErrNoSpaceOnCloud,
    };

    enum Status {
        kIdle,
        kDownloading,
        kUploading,
        kCustomSync,
    };

    struct File {
        std::string id;
        std::string title;
        std::string filename;
        int progress; // in percents
        int error;
        int downloaded_bytes;
        int file_size;

        File():progress(0),error(0),downloaded_bytes(0),file_size(0){}
        File(const std::string& id_p, const std::string& title_p, int progress_p = 0):id(id_p),title(title_p),progress(progress_p),error(0),downloaded_bytes(0),file_size(0){}

        bool operator==(const File& file) {
            return id == file.id;
        }

        int WriteToStream(std::ostream& stream) const;
        int ReadFromStream(std::istream& stream);
    };

    DownloadStatus();
    int Init(const std::string& service_name, int service_pid);
    int StartDownload();
    int SetProgressStatus(Status status);
    int StopDownload();
    void ResetProgress();
    int SetTotalProgressStatus(int files_total, int files_downloaded, int size_total = 0, int size_downloaded = 0);

    int SetErrorStatus(Error error, const std::string& error_description);

    int WriteToStream(std::ostream& stream) const;
    int ReadFromStream(std::istream& stream);

    std::string GetServiceName() const { return service_name_; }
    int GetServicePid() const { return service_pid_; }
    Status GetProgressStatus() const { return progress_status_; }

    Error GetError() const { return error_; }
    std::string GetErrorDescription() const { return error_description_; }

    int GetFilesTotal() const { return files_total_; }
    int GetFilesDownloaded() const { return files_downloaded_; }
    int GetSizeTotal() const { return size_total_; }
    int GetSizeDownloaded() const { return size_downloaded_; }
    File GetCurrentFile() const { return current_file_; }
    int GetCurrentProgress() const { return current_file_.progress; }

    int SetFileList(const std::vector<File>& files);
    std::vector<File> GetFileList();

    int SetCurrentProgressStatus(const File& updated_file_info);
    int SetCustomMessage(const std::string& msg);
    std::string GetCustomMessage() { return custom_message_; }
private:

    std::string service_name_;
    int service_pid_;
    Status progress_status_;
    Error error_;
    std::string error_description_;

    // Next fields valid only if is_in_process_ == true
    int files_total_;
    int files_downloaded_;
    int size_total_;     // 0 if unknown
    int size_downloaded_; // 0 if unknown
    File current_file_;
    std::vector<File> files_;
    std::string custom_message_;

};

class ServicesDownloadStatus
{
private:
    // It is singleton
    ServicesDownloadStatus();
    ServicesDownloadStatus(const ServicesDownloadStatus&);
    ServicesDownloadStatus& operator=(const ServicesDownloadStatus&);

public:

    static ServicesDownloadStatus& GetInstance();

    int UpdateStatus(const DownloadStatus& status);

    bool IsDownloading();

    std::vector<DownloadStatus> GetStatuses();

    ~ServicesDownloadStatus();

    static const std::string dropbox_name;
    static const std::string sync_name;
    static const std::string send_to_pb_name;
    static const std::string cloud_sync_name;
    static const std::string legimi_sync_name;

private:
    int SaveToFile();
    int LoadFromFile();
    int LockFile();
    void UnlockFile();

    bool IsDownloadingInt();
    DownloadStatus::Error IsErrorInt();

    int fd_;
    std::map<std::string, DownloadStatus> statuses_;
};

} // namespace utilities
} // namespace pocketbook

#endif // SERVICES_DOWNLOAD_STATUS_H
