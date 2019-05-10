#ifndef CONFIG_H
#define CONFIG_H

#include "configuration_watch.h"
#include "datetime.h"
#include <string>
#include <map>
#include <inkview.h>

namespace pocketbook {
namespace system {
namespace config {
bool IsReadRateForFileEnabled(const std::string & path);

class Conf {
public:
    Conf();
    Conf(const std::string &config_path);
    virtual ~Conf();
    void Open(const std::string &config_path);
    void Close();
    void Save();
    void WriteString(const std::string &key, const std::string &value, bool save_immediately = false);
    std::string ReadString(const std::string &key, const std::string &default_value = "");
    void WriteTime(const std::string &key, time_t value, bool save_immediately = false);
    time_t ReadTime(const std::string &key, time_t defaultTime = 0);
private:
    std::string m_path;
};

class Config
{
public:
    static Config * GetInstance();
    
    std::string ReadString(const std::string &config_path, const std::string &key, const std::string &default_value = "");
    std::map<std::string, std::string> ReadAllStrings(const std::string &config_path, const std::string &first_part_of_key);
    std::string ReadSecret(const std::string &config_path, const std::string &key, const std::string &default_value = "");
    int ReadInt(const std::string &config_path, const std::string &key, int default_value);
    void SaveConfig(const std::string &config_path, bool save_immediately = false);
    void CloseConfig(const std::string &config_path, bool need_save = false);
    void WriteInt(const std::string &config_path, const std::string &key, int value, bool save_immediately = false);
    void WriteString(const std::string &config_path, const std::string &key, const std::string &value, bool save_immediately = false);
    void WriteSecret(const std::string &config_path, const std::string &key, const std::string &value, bool save_immediately = false);
    void TrueWriteSecret(const std::string &config_path, const std::string &key, const std::string &value, bool save_immediately = false);

    void DeleteString(const std::string &config_path, const std::string &key, bool save_immediately = false);
    void Refresh(const std::string &config_path);
private:
    Config();
    ~Config();
    struct Listner : public pocketbook::utilities::ConfigurationMultiWatch::Notify {
        Listner(iconfig * cfg = NULL) : cfg_(cfg) {}
        void Changed() {if (cfg_) ::RefreshConfig(&cfg_);}
        void SetConfig(iconfig * cfg) {cfg_ = cfg;}
        iconfig *GetConfig() const {return cfg_;}
    private:
        iconfig *cfg_;
    };
    struct ConfigParam {
        ConfigParam() : cfg(NULL), listner(NULL), need_save(false) {}
        iconfig *cfg;
        Listner listner;
        bool need_save;
    };
    iconfig * OpenConfig(const std::string &config_path);
    iconfig * GetConfig(const std::string &config_path) const;
    ConfigParam *GetParam(const std::string &config_path) const;
    void SetSaveFlag(const std::string &config_path, bool flag);
    std::map<std::string, std::string> HashFind(const ihash *hash, const std::string &first_part_of_key);
    template <typename T1, typename T2> void CopyMapToMap(std::map<T1, T2> &dest_map, const std::map<T1, T2> &inserting_map) {
        for (typename std::map<T1, T2>::const_iterator it = inserting_map.begin(); it != inserting_map.end(); ++it) {
            dest_map[it->first] = it->second;
        }
    }

private:
    pocketbook::utilities::ConfigurationMultiWatch cfg_watch_;
    std::map<std::string, ConfigParam *> config_list_;
};

} // namespace config
} // namespace system
} // namespace pocketbook

#endif // CONFIG_H
