/**
  \brief This file contains Downloader class declaration. Main purpose for this class is to download files from specified url.
    It support aborting/resuming download
  \author Yuriy Lisovyy
  */
#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <pthread.h>

#include <string>
#include <vector>

namespace pocketbook
{
namespace utilities
{

/**
  \brief Callback interface
  */
struct IDownloaderCallback
{
    /**
      \brief Called each time downloader state changes
      */
    virtual void NotifyDownloaderStatus(void* user_data) = 0;
    virtual ~IDownloaderCallback(){}
};

class FileToSync
{
public:
    std::string download_url() const {return download_url_;}
    std::string file_name() const { return file_name_;}
    size_t file_size() const { return file_size_;}

    FileToSync(const std::string& url, const std::string& name, size_t size)
        :download_url_(url),file_name_(name),file_size_(size) {}

    FileToSync(){}
private:
    std::string download_url_;
    std::string file_name_;
    size_t      file_size_;
};

/**
  \brief Class manage download process
  */
class Downloader
{
public:

    /**
      \brief types of error during download
      */
    enum Error
    {
        DE_NoError,         /// All is OK
        DE_NetworkFailed,   /// Network failed
        DE_ServerError,     /// Server return error
        DE_NoSpace,         /// No disc space for operation
    };

    /**
      \brief status of the downloader object
      */
    enum Status{
        DS_Idle,    /// No item for download
        DS_Downloading, /// Downloading in progress
        DS_Finished,    /// Download finish successfully
        DS_Error,    /// Error during download
        DS_Paused,      /// Download paused (not used now)
        DS_Aborted      /// Download was aborted by user (through Abort() function)
    };

    /**
      \brief Status information structure
      */
    struct StatusInfo
    {
        Status status;
        Error error;
        int downloaded_bytes; /// How many bytes downloaded (valid for DS_Downloading)
        int response_code;
        int file_size;
        std::string content_disposition_filename_;
        std::string content_type_;
        std::string effective_url_;
    };

    Downloader();
    ~Downloader();
    /**
      \brief Initialize object. This function should be called only once after construction
       before any other function call
      \param cb callback to be called when status changed or commands execution complete
      \return 0 on succeed
      */
    int Init(IDownloaderCallback* cb);

    /**
      \brief Start download (asyncroniously). If part of the file was already downloaded it continue download.
      \param book book to download (fail in DS_Downloading and DS_Paused states). Can fail if another command in process
      \param downloadFolder folder to download book to
      \return 0 on succeed
      */
    int Start(const FileToSync& book, const std::string& downloadFolder, void* user_data);

    /**
      \brief Retry to download current book(asyncroniously)  (worked only in DS_NetError or DS_ServerResponceError states).
        Can fail if another command in process
      \param user_data data to be passed to callback when operation finished
      \return 0 on succeed
      */
    int Retry(void* user_data);

    /**
      \brief Abort download (asyncroniously). Work in all states and overwrite another command in process.
        Progress information is saved so trying downloading the file next time will continue download
      \param user_data data to be passed to callback when operation finished
      \return 0 on succeed
      */
    int Abort(void* user_data);

    /**
      \brief pause download (asyncroniously) (worked only in DS_Downloading state). Can fail if another command in process
      \param user_data data to be passed to callback when operation finished
      \return 0 on succeed
      */
    int Pause(void* user_data);
    /**
      \brief resume download (asyncroniously) (worked only in DS_Paused state). Can fail if another command in process
      \param user_data data to be passed to callback when operation finished
      \return 0 on succeed
      */
    int Resume(void* user_data);

    /**
      \brief get downloader status
      */
    StatusInfo GetStatus();
    /**
      \brief get current book
      */
    FileToSync GetBook();

    int ClearStatus();

    /**
      \brief delete progress information assiciated with file (trying to download the file after the call
        will cause it to be downloaded from beggining
      */
    void Cleanup(const FileToSync& book);

    void CleanupOnErr(const FileToSync& book);

    void SetCookie(const std::string& name, const std::string& value);

    void SetOverwriteFiles(bool value) { overwrite_files_ = value; }

    std::string GetFinalName() { return final_name_;}
    void AddHeader(const std::string& name, const std::string& value);
    void ClearHeaders();

    std::vector<char> getErrorBody(const FileToSync& book);
private:

    // Curl callbacks
    static int ProgressFunctionCaller(void *arg, double dltotal, double dlnow, double ultotal, double ulnow);
    int ProgressFunction(void *arg, double dltotal, double dlnow, double ultotal, double ulnow);
    static size_t WriteFunction( char *ptr, size_t size, size_t nmemb, void *userdata);
    static size_t WriteHeader( void *ptr, size_t size, size_t nmemb, void *userdata);

    // Thread which perform work
    static void* DownloadThread(void*);

    // Worker thread loop
    int StartProcess();

    // Start download command realization
    int StartDownload();

    Status status_; // Status of the object
    Error error_;     // Error type (valid only if status_ is DS_Error)
    int response_code_;
    std::string url_;   // Currect url to download from
    std::string folder_;    // folder to download to
    std::string filename_;  // downloaded file name
    int file_size_;         // file size (as determined by Book)
    int real_file_size_;
    IDownloaderCallback * cb_;  // Callback
    int downloaded_bytes_;  // Number of bytes already downloaded
    FileToSync book_;     // Current book
    void* user_data_;     // User data for completetion callback
    void* old_user_data_;  // User data for completetion callback for interrupted command (need to implement Abort which interrupt current command)
    std::string filename_from_header_;
    std::string cookie_;
    std::string content_type_;
    std::string effective_url_;
    std::string final_name_; // Full final name after cleanup

    // Commands used to communicate between calling and working thread
    enum Command {
        DC_None,
        DC_Download,
        DC_Abort,
        DC_Pause,
        DC_Resume,
        DC_Exit,
    };
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    Command command_;    
    pthread_t thread_;
    bool is_running_;
    int resume_from_;
    bool overwrite_files_;
    std::vector<std::string> headers_;

};

} // namespace utilities
} // namespace pocketbook

#endif // DOWNLOADER_H
