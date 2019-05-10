#ifndef FILE_TYPE_H
#define FILE_TYPE_H
#include <string>
#include <map>

namespace pocketbook
{
namespace utilities
{

struct FileType
{
    enum Type {
        kUnknown,
        kBook,
        kImage,
        kAudio,
        kApp,
        kWebLink,
        kZip,
        kFolder = 255,
    } type_;
    std::string ext_;
    std::string desc_;
    std::string run_app_;
    std::string icon_name_;

    FileType():type_(kUnknown){}
};

class FileTypeManager
{
public:
    FileTypeManager();

    int LoadTypesInfo(const std::string& filename);
    int LoadTypesInfo();

    FileType GetFileTypeByExt(const std::string& ext) const;
    FileType GetFileTypeByFilename(const std::string& filename) const;
    int ParseExtentionInfo(char* buf);

private:
    std::map<std::string, FileType> types_;
};

}
}
#endif // FILE_TYPE_H
