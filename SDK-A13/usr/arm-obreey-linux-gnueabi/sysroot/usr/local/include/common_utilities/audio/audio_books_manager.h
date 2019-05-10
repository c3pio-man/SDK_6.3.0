#ifndef AUDIO_BOOKS_MANAGER_H
#define AUDIO_BOOKS_MANAGER_H

#include <common_utilities/audio/audio_book.h>
#include <common_utilities/pb_sqlite.h>
#include <common_utilities/db_manager_helper.h>
#include <common_utilities/media_library_manager.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace pocketbook {
namespace audio {

class AudioBooksManager
{
public:
    struct AudioBookmark {
        int id_ = 0;
        int position_;
        std::string location_;
        std::string title_;
        std::string note_;
    };

    AudioBooksManager();

    void updateStorageIds();

    int addAudioBookByPath(const std::string &filepath);
    void createMp3AudioBooks(); // call it after add all mp3-files by addAudioBookByPath

    int addAudioBook(const std::shared_ptr<AudioBook> audiobook);
    int addFile(int64_t book_id, const std::string& filename);
    int addChapter(int64_t book_id, const AudioBook::Chapter& chapter);
    int setState(int64_t book_id, const AudioBook::State& state);
    enum class Order {
        Title,
        Author,
        Duration,
        TimeOpened,
    };

    int removeAudioBook(int64_t book_id);

    std::vector<std::shared_ptr<AudioBook> > getAudioBooks(Order order);
    std::vector<AudioBook::Chapter> getBookChapters(int64_t book_id);

    int addBookmark(int64_t book_id, const AudioBookmark &bookmark);
    int editBookmark(const AudioBookmark &bookmark);
    int removeBookmark(int bmk_id);
    std::vector<AudioBookmark> getBookmarks(int64_t book_id);

private:
    void scanDirectoryForOtherChapters(const std::string &path, const std::string &key, const std::string &extension);
    std::string getFolderFromPath(const std::string &path);
    bool isFileAlreadyAdded(const std::string &filepath);

    utilities::DbManagerHelper db_manager_helper_;
    utilities::Sqlite sqlite_;
    std::vector<int64_t> storage_ids_;

    utilities::MediaLibraryManager media_library_;
    std::map<std::string, std::vector<utilities::MediaLibraryManager::Track> > add_mp3_books_;

};

} // audio
} // pocketbook

#endif // AUDIO_BOOKS_MANAGER_H
