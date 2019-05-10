#ifndef CFGWRAPPER_H
#define CFGWRAPPER_H

#include <string>
#include "inkview.h"

namespace pocketbook {
namespace utilities {

struct CfgSecret {
    explicit CfgSecret(const std::string& s): value_(s){}
    CfgSecret(){}
    std::string value_;
};

class CfgWrapper
{
    CfgWrapper(const CfgWrapper&) = delete;
    CfgWrapper& operator=(CfgWrapper&) = delete;
public:
    enum OpenMode {
        kRead,
        kReadWrite,
    };

    CfgWrapper(const std::string& filename, OpenMode mode);
    ~CfgWrapper();
    template<typename T> T read(const std::string& name, T default_value = T()) const;


    void write(const std::string& name, const std::string& value) {
        WriteString(cfg_, name.c_str(), value.c_str());
    }

    void write(const std::string& name, int value) {
        WriteInt(cfg_, name.c_str(), value);
    }

    void write(const std::string& name, const CfgSecret& value) {
        WriteSecret(cfg_, name.c_str(), value.value_.c_str());
    }

    void remove(const std::string& name);
    void closeNoSave();
    void saveAndClose();
    void save();
    bool isOpened();
private:
    iconfig* cfg_ = nullptr;
    OpenMode mode_;
};

template<> inline int CfgWrapper::read<int>(const std::string& name, int default_value) const {
    return ReadInt(cfg_, name.c_str(), default_value);
}
template<> inline std::string CfgWrapper::read<std::string>(const std::string& name, std::string default_value) const {
    return ReadString(cfg_, name.c_str(), default_value.c_str());
}

template<> inline CfgSecret CfgWrapper::read<CfgSecret>(const std::string& name, CfgSecret default_value) const {
    return CfgSecret(ReadSecret(cfg_, name.c_str(), default_value.value_.c_str()));
}

template<typename T> inline T readFromConfig(const std::string& filename, const std::string& name, T defalut_value = T()) {
    CfgWrapper cfg(filename, CfgWrapper::kRead);
    return cfg.read<T>(name, defalut_value);
}

}} // namespace
#endif // CFGWRAPPER_H
