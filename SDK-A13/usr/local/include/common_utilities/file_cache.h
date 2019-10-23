#ifndef FILE_CACHE_H
#define FILE_CACHE_H

#include <string>
#include "pb_sqlite.h"
#include <vector>
#include <iostream>
#include <mutex>

namespace pocketbook {
namespace utilities {

class FileCache
{
public:
    FileCache(const std::string& dir, const std::string& cache_name);
    ~FileCache();

    std::string addFile(const std::string& key, const std::string& filepath, bool overwrite = false);
    std::string addData(const std::string& key, const std::vector<char>& data, bool overwrite = false);
    std::string addStream(const std::string& key, const std::string& label, const std::istream& data, bool overwrite = false);

    std::string getFile(const std::string& key, long long* size = nullptr);
    int removeFile(const std::string& key);
    int validate();
    int removeOlderThan(time_t ts);
    int clear();

    static const int c_default_max_size = 10 * 1024 * 1024;
    struct ClearParameter {
        int cache_max_size = c_default_max_size;
        int remove_older_than = 0;  // in days
        // int maximum_file_number = 0; // not supported yet
    };

    int setClearParameters(const ClearParameter&);

private:
    int Init();
    std::string getFileInt(const std::string& key, long long* size = nullptr);

    enum class LockType {
        Read,
        Write,
    };

    class Locker {
    public:
        Locker(FileCache* cache, LockType t):cache_(cache){cache_->lock(t);}
        ~Locker() { cache_->unlock(); }
        Locker(const Locker&) = delete;
        Locker& operator=(const Locker&) = delete;
    private:
        FileCache* cache_;
    };

    friend class Locker;
    int lock(LockType t);
    int unlock();
    int createScheme();
    int calcState();

    void UpdateFileAccessTime(long long id);
    std::string getChachedFullFilename(const std::string& key, const std::string& filepath);
    std::string dir_;
    std::string cache_name_;
    Sqlite sqlite_;
    long long total_size_ = 0;
    int number_of_files_ = 0;
    ClearParameter clear_params_;
    time_t last_validate_time_ = 0;
    int fd_ = -1;
    std::string lock_filename_;
    std::recursive_mutex mutex_;
};

}
}



#endif // FILE_CACHE_H
