#ifndef BROWSER_AUTHORIZATION_H
#define BROWSER_AUTHORIZATION_H

#include "common_utilities/message_server.h"
#include "common_utilities/message_queue.h"
#include "common_utilities/lock.h"
#include <string>

namespace pocketbook {
namespace utilities {

class BrowserAuthorization: public pocketbook::utilities::IMessageHandler
{
public:
    class IEventHandler {
    public:
        virtual void UrlChanged(const std::string& url) = 0;
        virtual void BrowserError() {}
        virtual void BrowserClosed() = 0;
        virtual void TimePassed(int time_ms) {}
        virtual ~IEventHandler() {}
    };

    BrowserAuthorization(const std::string& id, IEventHandler* handler);

    /**
      \brief Run browser in url inspecting mode and calling IEventHandler when browser state changed (url changed or browser closed).
        Function returns when browser whould be closed or after Stop function would be called.
      \param url url to open browser  whith
      \param additional_parameters to pass to browser (now supported {"-img", img_path}  - to show url in top of browser
      \return 0 - on success
      */
    int Start(const std::string& url, const std::vector<std::string>& additional_parameters);

    /**
      \brief Stop browser authorization and close browser. It isthread safe function
    */
    void Stop() { pocketbook::ScopedLock lock(&mutex_); is_running_ = false; }

    /**
      \brief Check is authorization in process. This is thread safe function
    */
    bool IsRunning()  { pocketbook::ScopedLock lock(&mutex_); return is_running_; }

    /**
      \brief Check is browser wich is used in authorization is in list of running tasks
    */
    bool IsBrowserApplicationRunning() const;

    /**
      \brief If authorization is in process move focus to browser. This is thread safe function
    */
    int SetFocus();
    void setBrowserAppName(const std::string& newName) {
        browser_app_name_ = newName;
    }

private:
    // overriden
    virtual void HandleMessage(pocketbook::utilities::IMessage * message);

    void ClearParamsUnsafe();
    std::vector<char*> CopyParamsUnsafe();


    std::string id_;
    IEventHandler* handler_;
    pthread_mutex_t mutex_;
    bool is_running_;
    bool is_closed_by_browser_;
    std::vector<char*> params_;
    std::string browser_app_name_;
    int running_time_ms_;
};

} // namespace utilities
} // namespace pocketbook

#endif // BROWSER_AUTHORIZATION_H
