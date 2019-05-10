#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>
#include <sys/vfs.h>



namespace pocketbook {
namespace system {
namespace filesystem {

/**
  * \brief Function find all files by extensions in directory 
  * \param directory - path to folder, where files must be found
  * \param extention - list of extensions separated by semicolons ("exe,app,sh"). 
  *                    If extension = * , Function find all files in directory
  * \return return list of matched files in directory
**/
std::vector<std::string> GetFiles(const std::string &directory, const std::string &extension);
std::vector<std::string> GetFiles(const std::vector<std::string> &directories, const std::string &extension);

/**
  * \brief Function analize full filename and return filename with out extention
  * \param name - filename. Must be with out any part of file path (filename.app)
  * \return filename with out extention
**/
std::string GetShortFileName (const std::string &name);
std::vector<std::string> GetShortFileName (const std::vector<std::string> &names);

/**
  * \brief Function analize full path to file and return path with filename
  * \param path - path to file
  * \return filename with extention
**/
std::string GetFileNameFromPath(const std::string &path);

/**
  * \brief Function analize full path to file and return only a directory part
  * \param path - path to file
  * \return directory part
**/
std::string GetDirectoryTreeFromPath(const std::string &path);

/**
  * \brief function check for available SD-card
  * \param - path for SD-card (/mnt/ext2)
  * \return return true if SD-card is mounted or false in otherwise
**/
bool IsSDCardMounted(const std::string &path);

/**
  * \brief Function try to found file in specified directory and return result
  * \param path_to_file - full path to file (/mnt/ext1/system/global.cfg)
  * \return true if file exist or false in otherwise
**/
bool IsFileExist(const std::string &path_to_file);

/**
  * \brief Function compare extension of file with specified extestions and return resul
  * \param name -
  * \param extension - list of extesions separated by semicolons (exe,app,sh) or * for all extensions
  * \return true if extension is coincide or false in otherwise
**/
bool MatchExtention(const std::string &name, const std::string &extension);

std::string GetExtentionFromPath(const std::string &path);

/**
  * \brief Function move all files from source dir to destination directory. Function automaticaly escape
           special symbols.
  * \param dest_dir - destination directory
  * \param source_dir - source directory
  * \param del_source_dir - if this paramater is true, source dir will be removed after moving all files 
  * \return 0 if moving succeeded or error number in otherwise
**/
int MoveFiles(const std::string &dest_dir, const std::string &source_dir, bool del_source_dir = false);

/**
  * \brief Function move file to destination directory. Function automaticaly escape
           special symbols.
  * \param file - file to move
  * \param dest_file - destination directory or file
  * \return 0 if moving succeeded or error number in otherwise
**/
int RenameDir(const std::string &src_dir, const std::string &dest_dir);
int MoveFile(const std::string &file, const std::string &dest_file);

/**
  * \brief Function copy file to destination directory or file. Function automaticaly escape
           special symbols.
  * \param file - file to copy
  * \param dest_file - destination directory or file
  * \return 0 if moving succeeded or error number in otherwise
**/
int CopyFile(const std::string &file, const std::string &dest_file);

/**
  * \brief Remove all files from specified directory
  * \param source_dir - source dir
  * \return 0 if removing succeeded or error number in otherwise
**/
int RemoveAllFiles(const std::string &source_dir);

/**
  * \brief Remove file
  * \param file - path to file
  * \return 0 if removing succeeded or error number in otherwise
**/
int RemoveFile(const std::string &file);

/**
  * \brief Remove specified directory
  * \param source_dir - source directory
  * \return 0 if removing succeeded or error number in otherwise
**/
int RemoveDir(const std::string &source_dir);

/**
  * \brief Run system command and return std out of this command
  * \param command - system command ("ls -l source_dir | grep lib")
  * \return std out if command executed successfully or empty string in otherwise
**/
std::string System(const std::string &command);

/**
  * \brief Function compare path to Internal SD-card with specified path
  * \param path - file path or path to directory
  * \return true if path located at Internal memory or false in otherwise
**/
bool IsInternalLocation(const std::string &path);

/**
  * \brief Function compare path to External SD-card with specified path
  * \param path - file path or path to directory
  * \return true if path has located at External memory or false in otherwise
**/
bool IsExternalLocation(const std::string &path);

/**
  * \brief Function remove path to Internal or External SD-Card from specified path 
  * \param path - file path or path to directory
  * \return path with out SD-Card prefix
**/
std::string CutCardPrefix(const std::string &path);

/**
  * \brief Function remove SD-card prefix and replace it to human
  * \param path - file path or path to directory
  * \return path with human SD-card name
**/
std::string GetPrintableFolderName(const std::string& folder_name);

/**
 * @brief AddEscapeSymbols - function find in source string a special system symbols that must to be escaped and escape it's.
 * @param str - string to parse
 * @return - return string with escaped special system symbols
 */
std::string AddEscapeSymbols(const std::string &str);

/**
 * @brief Function create directory tree
 * @param dir_tree - directory tree (like: /mnt/ext1/system)
 * @return - return 0 if directory tree was created or -1 in otherwise
 */
int CreateDirectoryTree(const std::string& dir_tree);

/**
 * @brief Function returns the free disk space on
 * @param destPath - the specified location
 * @return size in MB
 */
int GetFreeSpace(const std::string& destPath);

class FileSystem
{
public:
    FileSystem();
    
    // Remove this 3 functions
    int ReadFile(const std::string &file_name, std::vector<char> &buffer);
    int ReadFile(const std::string &file_name, std::vector<std::string> &buffer);
    int ReadFile(const std::string &file_name, std::string &buffer);
    
    int ReadFile(const std::string &file_name, std::vector<char> *buffer);
    int ReadFile(const std::string &file_name, std::vector<std::string> *buffer);
    int ReadFile(const std::string &file_name, std::string *buffer);
    
    int WriteFile(std::string file_name, const std::string &buffer);
    
    int Find(const std::string &pattern);
    int Find(const std::string &pattern, size_t row);
    int FindNext();
private:
    std::vector<std::string> file_buffer_;
    size_t find_row_;
    std::string search_pattern_;
};

} // namespace filesystem
} // namespace system
} // namespace pocketbook

#endif // FILESYSTEM_H
