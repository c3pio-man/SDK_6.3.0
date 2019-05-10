#ifndef UUID_H
#define UUID_H

#include <string>
namespace pocketbook {
namespace utilities {

/**
    GUIDs are usually stored as 128-bit values, and are commonly displayed as 32 hexadecimal digits with groups
    separated by hyphens, such as:
    21EC2020-3AEA-4069-A2DD-08002B30309D
 */
std::string GenerateUUID();

} // namespace system
} // namespace pocketbook

#endif // UUID_H
