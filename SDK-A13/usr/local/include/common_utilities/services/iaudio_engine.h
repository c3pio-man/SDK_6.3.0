#ifndef IAUDIO_ENGINE_H
#define IAUDIO_ENGINE_H

#include <vector>
#include <string>
#include <map>
#include <functional>
#include "inkview.h"

namespace pocketbook {
namespace utilities {


class IAudioEngineStream {
public:
    virtual ~IAudioEngineStream() {}
    virtual int doReadPacket(uint8_t *buf, int buf_size) = 0;
    virtual int64_t doSeek(int64_t offset, int whence) = 0;
};

class IAudioEngine {

public:
    enum State {
        kStopped,
        kPlaying,
        kPaused,
        kFinished,
    };

    enum class ErrorType {
        FailedOpenAlsa,
    };

    virtual void setStateChangedCb(std::function<void()> state_changed_cb) = 0;
    virtual void setErrorCallback(std::function<void(ErrorType)> error_cb)  = 0;

    virtual int openFile(const std::string& filename) = 0;
    virtual int openWithStream(IAudioEngineStream* stream) = 0;
    virtual void closeFile() = 0;
    virtual int start() = 0;
    virtual int stop() = 0;
    virtual int pause() = 0;
    virtual int resume() = 0;
    virtual void seekStream(int time_ms) = 0;

    virtual State getState() = 0;
    virtual int64_t getPosition() = 0;
    virtual bool isFileOpened() = 0;
    virtual void setTempo(double value) = 0;

    virtual double getTempo() = 0;

    virtual void setSampleRateMultiplier(double value) = 0;

    virtual double getSampleRateMultiplier() = 0;

    virtual int64_t getDurationMs() = 0;

    virtual void setVolume(int value) = 0;
    virtual int getVolume() = 0;
    virtual void setEqualizerSettings(std::vector<std::pair<int, int> > s) = 0;

    struct Chapter {
        int64_t start, end;     ///< chapter start/end time in ms
        std::map<std::string, std::string> metadata_;
    };

    virtual std::map<std::string, std::string> getMetadata() = 0;

    virtual std::vector<Chapter> getChapters() = 0;

    virtual ibitmap* getCover() = 0;
    virtual ~IAudioEngine() {}
};

IAudioEngine* createAudioEngine();


}}
#endif // IAUDIO_ENGINE_H
