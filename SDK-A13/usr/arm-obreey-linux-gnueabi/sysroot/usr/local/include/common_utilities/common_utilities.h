#ifndef COMMON_UTILITIES_H
#define COMMON_UTILITIES_H

#include <string>

namespace pocketbook {
namespace utilities {

// return 0 - successfull, 1 - error
int RunAdobeScanner(const std::string& file_path, const std::string& png_path, int width = 0, int height = 0, bool metadata = true);

} // namespace utilities
} // namespace pocketbook

#endif // COMMON_UTILITIES_H
