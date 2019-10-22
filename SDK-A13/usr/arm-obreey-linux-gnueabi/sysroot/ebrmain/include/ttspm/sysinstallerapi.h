#ifndef SYSINSTALLERAPI_H
#define SYSINSTALLERAPI_H

#include <QObject>

namespace pocketbook {
namespace ttspm {

typedef void (*td_notify_func)(const int percent_complite);
typedef void (*td_complete_func)(const int error_code);

class SysInstallerApi : public QObject
{
    Q_OBJECT

public:
    static SysInstallerApi& Instance();

    bool init();
    bool load_library();
    bool install_pack(const std::string &full_package_path);
    bool uninstall_pack(const std::string &package_name);

signals:
    void notifyInstallProgress(const int percent);
    void notifyInstallComplete(const int error_code);
    void notifyUninstallComplete(const int error_code);

private:
    SysInstallerApi();
    bool prepare();

    static void stat_notifyInstallProgress(const int percent);
    static void stat_notifyCommandCompleted(const int error_code);
    static void* stat_thread(void* parameter);
    void main_thread_loop();

    void fireInstallNotify(const int percent);
    void commandCompleted(const int error_code);
    void fireInstallComplete(const int error_code);
    void fireUninstallComplete(const int error_code);

    enum class Commands {
        IC_None,
        IC_Install,
        IC_Uninstall,
        IC_Exit
    } command_ = Commands::IC_None;
    Commands last_command_ = Commands::IC_None;

    int current_percent_ = -1;
    std::string package_to_proceed_;
    bool is_init_ = false;
    bool is_lib_loaded_ = false;
    void *lib_ptr_ = nullptr;
    int (*install_proc)(const char *full_package_path) = nullptr;
    int (*uninstall_proc)(const char *package_name) = nullptr;
    void (*subsribe_notify_proc)(const td_notify_func callback_func) = nullptr;
    void (*subscribe_complete_proc)(const td_complete_func callback_complete_func) = nullptr;

    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    pthread_t thread_;
};

#define AppSysInstallerApi SysInstallerApi::Instance()

}
}

#endif // SYSINSTALLERAPI_H
