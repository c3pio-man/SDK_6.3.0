#ifndef BROWSER_INTEGRATION_INT_H
#define BROWSER_INTEGRATION_INT_H

#define BROWSER_NOTIFICATION_SOCKET_NAME "\0browser_notification"
#define MAX_BROWSER_NOTIFICATION_MESSAGE_SIZE 4 * 4096

namespace pocketbook {
namespace utilities {

struct BrowserMessageTypes {
    enum Type {
        kUrlChanged,
        kClosed,
        kBrowserError,
    };
};

} // namespace utilities
} // namespace pocketbook
#endif // BROWSER_INTEGRATION_INT_H
