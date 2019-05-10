#ifndef MEDIA_LIBRARY_MANAGER_H
#define MEDIA_LIBRARY_MANAGER_H

#include <common_utilities/pb_sqlite.h>
#include <string>
#include <map>
#include <vector>
#include "common_utilities/db_manager_helper.h"

namespace pocketbook {
namespace utilities {

class MediaLibraryManager
{
public:
    struct Track
    {
        int64_t id_ = 0;
        std::string filename_;
        std::string original_filename_; // only for tracks loaded from m3u
        int duration_ = 0;
        std::string title_;
        std::string artist_;
        std::string album_;
        int track_no_ = 0;
        std::string genre_;
        int size_ = 0;
        std::string fast_hash_;
        std::map<std::string, std::string> props_;
        bool is_metadata_extracted_ = false;
        int timestamp_ = 0;
        Track(const char* filename, const char * title):filename_(filename),title_(title){}
        Track() {}
    };

    enum TrackGroupType {
        kArtist,
        kAlbum,
        kGenre,
        kFolder,
        kAll,
    };

    enum SortDir {
        kAsc,
        kDesc,
    };

    struct TrackGroup {
        TrackGroupType type_;
        std::string name_;
        int items_num_;
    };

    MediaLibraryManager();

    enum SortType {
        kTitle,
        kTrackNo,
        kFileName,
        kLength,
        kDate
    };

    std::vector<Track> getTracks(SortType sort, TrackGroupType filter_type, std::string filter_value);
    std::vector<TrackGroup> getTrackGroups(TrackGroupType group_type, SortDir dir);
    std::vector<Track> search(const std::string &text);
    void updateStorageIds();
    int addTrack(Track track);
    int removeTrack(int64_t track_id);
    Track getTrackByPath(const std::string &filepath);
    static int GetDb(pocketbook::utilities::Sqlite& sqlite);

    static void getFileTags(MediaLibraryManager::Track& track);

    static std::vector<Track> playListFromDirectory(const char* path, bool handle_play_list);
    static std::vector<Track> LoadPlayList(const std::string& path);
    static int SavePlayList(const std::vector<Track>& list, const std::string& path);

private:
    int GetDb(pocketbook::utilities::Sqlite& sqlite, const std::string& filename);
    void init();

    Sqlite sqlite_;
    std::vector<int64_t> storage_ids_;
    static std::string encoding_;
    DbManagerHelper db_manager_helper_;
    std::string custom_collation_;
};


}
}


#endif // MEDIA_LIBRARY_MANAGER_H
