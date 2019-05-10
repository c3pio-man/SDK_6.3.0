#ifndef BLUETOOTHDOWNLOADINFO_H
#define BLUETOOTHDOWNLOADINFO_H

#include "pb_sqlite.h"
#include <vector>

namespace pocketbook {
namespace utilities {

struct FileToSend {
    std::string filename_;
    std::string address_;
    FileToSend(const std::string& filename, const std::string& address):filename_(filename),address_(address) {}
    FileToSend(){}
    FileToSend(const FileToSend&) = default;
    bool operator<(const FileToSend& r) { return filename_ < r.filename_ || (filename_ == r.filename_ && address_ == r.address_);}
};

typedef std::vector<FileToSend> PendingFiles;

class BluetoothDownloadInfo
{
public:
public:
    enum Status {
        kPending = 0,
        kUploading = 1,
        kUploaded = 2,
        kDownloading = 3,
        kDownloaded = 4,
        kError = 5,
        kCancelled = 6,
        kHidden = 7,
    };

    static std::string GetDefaultFilename();
    BluetoothDownloadInfo(const std::string& filename);
    PendingFiles GetPendingFiles();
    struct File {
        std::string filename_;
        std::string address_;
        Status status_;
        uint64_t size_ = 0;
        uint64_t downloaded_ = 0;
        uint64_t timestamp_ = 0;
        File(const std::string& filename, const std::string& address, Status status):filename_(filename),
            address_(address),status_(status){}
    };
    std::vector<File> GetFiles();

    sqlite_int64 AddPendingFile(const FileToSend& file, sqlite3_int64 file_size = 0);
    void ChangeFileStatus(const FileToSend& file, Status status);
    void SetProgress(const FileToSend& file, sqlite_int64 size, sqlite_int64 uploaded);
    Status GetFileStatus(sqlite3_int64 id);
    void SetFileStatus(sqlite3_int64 id, Status status);
    std::string GetFilePath(sqlite3_int64 id);
private:
    Sqlite sqlite_;
};

}
}

#endif // BLUETOOTHDOWNLOADINFO_H
