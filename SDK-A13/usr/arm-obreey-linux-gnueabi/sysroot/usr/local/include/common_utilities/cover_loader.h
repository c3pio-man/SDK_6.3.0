#ifndef COVER_LOADER_H
#define COVER_LOADER_H

#include "inkview.h"
#include "file_type.h"
#include <string>
#include <set>
#include <common_utilities/smartbitmap.h>

namespace pocketbook
{
namespace utilities
{

class CoverLoader
{
public:
    CoverLoader();

    enum LoadResult {
        kOk,
        kTryLater,
        kNoCover,
        kFailed,
    };

    LoadResult LoadCoverByHash(const std::string& hash, int hash_type, int width, int height, int load_flags, pocketbook::utilities::SmartBitmap *cover);
    LoadResult LoadCoverByHashFromCache(const std::string& hash, int hash_type, int width, int height, int load_flags, pocketbook::utilities::SmartBitmap *cover);

    LoadResult LoadCover(const std::string& path, int width, int height, int load_flags, pocketbook::utilities::SmartBitmap *cover);
    LoadResult LoadCoverForceReload(const std::string& path, int width, int height, int load_flags, pocketbook::utilities::SmartBitmap *cover);
    LoadResult LoadCoverEx(const std::string& path, int width, int height, int load_flags, pocketbook::utilities::SmartBitmap *cover, bool force_reload = false);

    LoadResult LoadCoverFromCache(const std::string& path, int width, int height, int load_flags, pocketbook::utilities::SmartBitmap *cover);

    std::string GetImagePath(const std::string& book_path);
    std::string GetImagePathByHash(const std::string& book_hash, int hash_type);
    int GetCachedWidth() { return cached_width_; }
    int GetCachedHeight() { return cached_height_; }
private:

    LoadResult LoadCoverInt(const std::string& path, const std::string& png_path, pocketbook::utilities::SmartBitmap * cover);

    pocketbook::utilities::SmartBitmap GetImageCover(const std::string& fullpath, const std::string& ext);
    CoverLoader::LoadResult EnqueueToAdobe(const std::string& path, const std::string& png_path);

    FileTypeManager ft_manager_;
    std::string chache_path_;
    int cached_width_;
    int cached_height_;
    std::set<std::string> queued_items_;
};

}
}
#endif // COVER_LOADER_H
