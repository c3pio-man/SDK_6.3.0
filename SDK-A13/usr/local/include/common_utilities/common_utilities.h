#ifndef COMMON_UTILITIES_H
#define COMMON_UTILITIES_H

#include <string>

namespace pocketbook {
namespace utilities {

// return 0 - successfull, 1 - error
int RunAdobeScanner(const std::string& file_path, const std::string& png_path, int width = 0, int height = 0, bool metadata = true);

// show global system message in current active task
// any task can call it, but message shows as Message in active task
void SystemMessage(int icon, const std::string title, const std::string text, int timeout);

// show global system dialog in current active task
// any task can call it, but dialog shows as dialog in active task
// return number of choosed button
int SystemDialog(int icon, const std::string title, const std::string text,
                            const std::string button1, const std::string button2);

// any task can call it, send signal to open book from active task
void SystemOpenBook(const std::string &path);

} // namespace utilities
} // namespace pocketbook

#endif // COMMON_UTILITIES_H
