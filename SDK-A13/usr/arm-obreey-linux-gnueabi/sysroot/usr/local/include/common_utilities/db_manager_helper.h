#ifndef DB_MANAGER_HELPER_H
#define DB_MANAGER_HELPER_H

#include <common_utilities/pb_sqlite.h>
#include <string>
#include <map>
#include <vector>

namespace pocketbook {
namespace utilities {


class DbManagerHelper
{
public:
    DbManagerHelper(const std::string& filename, const std::string& corruption_flag_name,
                    int version, const std::string& version_scripts_filename,
                    const std::vector<std::string>& create_requests);
    int GetDb(pocketbook::utilities::Sqlite& sqlite);
    int GetDb(pocketbook::utilities::Sqlite& sqlite, const std::string& filename);

private:
    int CurrentDbVersion();
    int CreateSchema(pocketbook::utilities::Sqlite& sqlite);
    void CorruptHandler(void);
    int GetVersion(pocketbook::utilities::Sqlite& sqlite);
    bool TestVersion(pocketbook::utilities::Sqlite& sqlite);
    int Setup(pocketbook::utilities::Sqlite& sqlite);
    int FixVersion(pocketbook::utilities::Sqlite& sqlite);

    std::string filename_;
    std::string corrupt_flag_name_;
    int version_ = 0;
    std::string version_scripts_filename_;
    std::vector<std::string> create_requests_;

};

class StorageHelper {
public:
    StorageHelper(pocketbook::utilities::Sqlite & sqlite);
    std::vector<int64_t> getStorages();
    int64_t GetStorageByPath(const std::string& path);
    int64_t GetStorageByName(const std::string& name);

private:
    pocketbook::utilities::Sqlite & sqlite_;
};
}
}
#endif // DB_MANAGER_HELPER_H
