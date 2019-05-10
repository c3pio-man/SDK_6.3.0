#ifndef PB_ZIP_MANAGER_H
#define PB_ZIP_MANAGER_H

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

namespace pocketbook
{

enum {
    PBZM_OK = 0,
    PBZM_NOT_OPENED = -1,
    PBZM_BAD_FILE_PATH = -2,
    PBZM_RESOURCE_BUSY = -3,
    PBZM_CURRENT_NOT_OPENED = -4,
    PBZM_BAD_INPUT_PARAMS = -5,
    PBZM_ERROR_GET_INFO = -6,
}; // pbzm_error

class PBZipManager
{
public:
    PBZipManager();
    ~PBZipManager();

    /**
     * @brief Open - open archive by zip_file_path
     * @param zip_filepath - path to archive
     * @param current_file - path of file in archive
     * @return errors from enum pbzm_error
     */
    int Open(const std::string &zip_filepath, const std::string &current_file = "");

    /**
     * @brief Close - close current archive
     * @return errors from enum pbzm_error
     */
    int Close();

    /**
     * @brief Read - read data from current file in archive
     * @param buf - output buffer for read data
     * @param size - size of read data
     * @return size of read data
     */
    int Read(void *buf, size_t size);

    /**
     * @brief ChangeFile - change current opened file in archive
     * @param current_file - file path in archive
     * @param relative - ???
     * @return errors from enum pbzm_error
     */
    int ChangeFile(const std::string &current_file);

    /**
     * @brief Skip - skip data in current file
     * @param size - size of skipping bytes
     * @return skipped count bytes
     */
    int Skip(size_t size);

    /**
     * @brief Tell - current position in current file in archive
     * @return position
     */
    int Tell() const;

    /**
     * @brief IsOpen
     * @return true - if archive file is opened
     */
    bool IsOpen() const;

    /**
     * @brief IsCurrentOpen
     * @return true - if archive and file in archive opened
     */
    bool IsCurrentOpen() const;

    /**
     * @brief GetCurrentFileSize
     * @return current file size
     */
    long int GetCurrentFileSize() const;

    /**
     * @brief GetFilePath
     * @return zip_file_path_
     */
    const std::string &GetFilePath() const { return zip_file_path_; }

    /**
     * @brief GetCurrentFilePath
     * @return current_file_;
     */
    const std::string &GetCurrentFilePath() const { return current_file_; }

    int GetFilesList(std::vector<std::string> &list);

private:
    /**
     * @brief zip_f_ - pointer to unzFile
     */
    typedef void* unzFile;
    unzFile zip_f_ = nullptr;

    /**
     * @brief zip_file_path_ - file path to archive
     */
    std::string zip_file_path_;

    /**
     * @brief current_file_ - path to current file in archive
     */
    std::string current_file_;
};

} // pocketbook

#endif // PB_ZIP_MANAGER_H
