#ifndef CONFIGURATION_WATCH_H
#define CONFIGURATION_WATCH_H

#include <string>
#include "inkview.h"
#include <map>

namespace pocketbook {
namespace utilities {

class ConfigurationWatch
{
public:

    struct Notify
    {
    public:
        virtual void Changed() = 0;
        virtual ~Notify(){}
    };

    ConfigurationWatch();
    ~ConfigurationWatch();

    int Init(const std::string& filename, Notify* listener);

    void CheckChanges();

    int ReadInt(const char* name, int deflt) {return ::ReadInt(cfg_, name, deflt); }
    const char* ReadString(const char *name, const char *deflt)  { return ::ReadString(cfg_, name, deflt);}
    const char *ReadSecret(const char *name, const char *deflt) { return ::ReadSecret(cfg_, name, deflt); }

private:
    std::string filename_;
    int notify_fd_;
    int watch_fd_;
    Notify* listener_;
    iconfig* cfg_;

};

class ConfigurationMultiWatch {

public:
    struct Notify
    {
    public:
        virtual void Changed() = 0;
        virtual ~Notify(){}
    };
    
    ConfigurationMultiWatch();
    ~ConfigurationMultiWatch();
    
    int AddWatch(const std::string &file, Notify *listner = NULL);
    int DeleteWatch(const std::string &file, const Notify *listner = NULL);
    void CheckChanges();
    
private:
    struct WatchInfo {
        WatchInfo() : cfg(NULL), watch_wd(0), listner(NULL) {}
        iconfig *cfg;
        int watch_wd;
        Notify *listner;
    };
    int DeleteWatch(std::multimap<std::string, WatchInfo *>::iterator &it);
    int notify_fd_;
    std::multimap<std::string, WatchInfo *> watched_list_;
};

} // namespace utilities
} // namespace pocketbook
#endif // CONFIGURATION_WATCH_H
