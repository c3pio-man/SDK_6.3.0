#ifndef IPLAYER_H
#define IPLAYER_H

#include "common_utilities/media_library_manager.h"
#include "common_utilities/audio/audio_books_manager.h"
#include "inkview.h"

namespace pocketbook {
namespace utilities {

class PlayerCb {
public:
    enum Error {
        kNoSuchFile,
    };

    virtual void TrackChanged() = 0;
    virtual void ErrorOccurs(Error err) = 0;
    virtual void stateChanged() = 0; // can be called from another thread

    virtual ~PlayerCb(){}
};

class IPlayer
{
public:
    using Track = MediaLibraryManager::Track;
    typedef std::vector<MediaLibraryManager::Track> PlayList;

    virtual ~IPlayer() {}
    enum PlayState {
        kStopped,
        kPlayed,
        kPaused,
        kFinished,
    };

    enum PlayMode {
        kRepeatOneTrack,
        kRepeatPlaylist,
        kSequentally,
        kRandom,
    };

    virtual void SetPlayList(const PlayList& playlist) = 0;

    virtual int PlayTrack(int n, int position = -1, bool pause_after_start = false) = 0;
    virtual int GetCurrentTrack() = 0;

    virtual void PreviousTrack() = 0;
    virtual void NextTrack() = 0;

    virtual void SetMode(PlayMode mode) = 0;
    virtual PlayMode GetMode() = 0;

    virtual int GetTrackSize() = 0;
    virtual void SetTrackPosition(int pos) = 0;
    virtual int GetTrackPosition() = 0;

    virtual void SetState(PlayState state) = 0;
    virtual int GetState() = 0;
    virtual void TogglePlaying() = 0;

    virtual void SetVolume(int n) = 0;
    virtual int GetVolume() =0;
    virtual int GetMaxVolume() = 0;

    virtual void HandlePlayerStateChanged() = 0;

    virtual void EnqueueTrack(size_t num) = 0;
    virtual void DequeueTrack(size_t num) = 0;
    virtual int GetTrackEnqueueIndex(size_t num) = 0;   // return -1 if track is not enqueued

    virtual Track GetPlayingTrack() = 0;
    virtual void setCb(PlayerCb* cb) = 0;

    virtual void setTempo(double tempo) = 0;
    virtual double getTempo() = 0;
    virtual void setSampleRateMultiplier(double value) = 0;
    virtual double getSampleRateMultiplier() = 0;
    virtual void setEqualizerSettings(std::vector<std::pair<int, int> > s) = 0;

    virtual std::map<std::string, std::string> getMetadata() = 0;
    virtual std::vector<pocketbook::audio::AudioBook::Chapter> getChapters() = 0;

    virtual ibitmap* getCover() = 0;
    virtual int openFile(const std::string& filename) = 0;

};

IPlayer* createPlayer();
}
}
#endif // IPLAYER_H
