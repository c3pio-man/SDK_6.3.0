#ifndef AUDIO_BOOK_H
#define AUDIO_BOOK_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "../media_library_manager.h"

namespace pocketbook {
namespace audio {

class AudioBook
{
public:
    typedef std::vector<utilities::MediaLibraryManager::Track> PlayList;
    AudioBook();
    virtual ~AudioBook();

    enum AudioBookType {
        kUnknown = 0,
        kEmpty   = 1,
        kM4aM4b  = 2,
        kMp3     = 3,
        kMp3Zip  = 4,
    };

    struct Chapter {
        int no_ = -1;
        std::string filename_;
        std::string title_;
        int start_position_ = 0; // ms
        int end_position_ = 0; // ms
    };

    struct State {
        State(const std::string& position, int percents, int64_t ts)
            : position_(position)
            , percents_(percents)
            , time_opened_(ts)
        {}
        State(){}
        std::string position_;
        int percents_ = 0;
        int64_t time_opened_ = 0;
    };

    AudioBookType getType()
    {
        return type_;
    }

    static std::shared_ptr<AudioBook> factory(AudioBookType type);

    void setId(int64_t id);
    int64_t getId();

    void setTitle(const std::string &title);
    std::string getTitle();

    void setAuthor(const std::string &author);
    std::string getAuthor();

    void setGenre(const std::string &genre);
    std::string getGenre();

    void setDuration(int duration);
    int getDuration();

    void addChapter(Chapter &chapter);
    void setChapters(std::vector<Chapter> chapters);
    std::vector<Chapter> getChapters();
    const Chapter getChapter(size_t index);
    int getChapterDuration(size_t index); // sec

    void setState(State state);
    State getState();

    float getBookPercents(int track, int position);

    virtual PlayList getBookPlaylist() = 0;
    virtual int getChapterIndex(int track, int position) = 0;
    virtual std::tuple<int /*track*/, int /*position*/> getChapterPlayInfo(size_t chapter) = 0;
    virtual std::string getBookPath() = 0;
    virtual bool removeBookFromDevice() = 0;
    virtual int getPositionInBook(int track, int position) = 0;
    virtual std::tuple<int /*track*/, int /*position*/> getPositionInTrack(int book_position) = 0;
    virtual std::string positionToLocation(int track, int position) = 0;
    virtual std::tuple<int /*track*/, int /*position*/> locationToPosition(const std::string &location) = 0;
    virtual int64_t getBookSize() = 0;

private:
    int64_t id_ = 0;
    std::string title_;
    std::string author_;
    std::string genre_;
    int duration_ = 0; //sec
    std::vector<Chapter> chapters_;
    State state_;

private:
    AudioBookType type_ = kUnknown;
};

} // audio
} // pocketbook

#endif // AUDIO_BOOK_H
