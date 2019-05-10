#pragma once

#include <stdio.h>
#include <string>
#include <map>
#include <functional>
#include <dlfcn.h>

namespace pocketbook {
namespace utilities {

/**
 * @brief class load library by dlopen. Can run different functions fro this library.
 * If function or library do not found, it throw exeption runtime_error.
 */
 
class LibraryLauncher
{
public:
    enum eFunction {
        kSetParameters = 0,
        kRun,
        kGetStatus,
        kGetResult,
        kSetNotificationFlag,
        kAddNotification
    };

    LibraryLauncher(const std::string &lib_path);
    
    ~LibraryLauncher();
    
    /**
     * @brief load library in memory
     * @return true if successful
     */
     
    bool LoadLibrary(const int flags = RTLD_LAZY);
    
    /**
     * @brief Change standart function name (such as SetParameters, Run, GetStatus or GetResult)
     * @param fn_id - function id (eFunction enum)
     * @param new_name - function name
     */
    void ChangeFunctionName(LibraryLauncher::eFunction fn_id, const std::string & new_name);
    
    /**
     * @brief U can set parameters in library before run it (look like main(int argc, char **argv))
     * @param argc - count of arguments
     * @param argv - array of arguments
     */
    void SetParameters(int argc, const char * const *argv);
    
    /**
     * @brief switch current handler to library frame
     */
    void Run();
    
    /**
     * @brief gel library internal status.
     */
    int GetStatus();
    
    /**
     * @brief get library working result. Call this function, when library was closed
     */
    int GetResult();
    
    /**
     * @brief Set notification flag in module if notification is available
     */
    void SetNotificationFlag(bool flag);
    
    /**
     * @brief Add notification
     */
    void AddNotification();
    
    /**
     * @brief U can run a void function in library
     * @param f_name - function name to run
     * @param argv - function parameters
     */
    template<typename F, typename ... ARGV>
    void DoVoidFunction(const std::string &f_name, ARGV ... argv)
    {
        if(!lib_) throw std::runtime_error("Library does not load or found");
        std::function<F> fn = (F*) ::dlsym(lib_, f_name.c_str());
        if(!fn) throw std::runtime_error("Function does not found");
        fn(argv ...);
    }
    
    /**
     * @brief U can run different function in library
     * @param f_name - function name to run
     * @param argv - function parameters
     */
    template<typename F, typename ... ARGV>
    auto DoFunction(const std::string &f_name, ARGV ... argv) -> decltype(std::declval<F>()(argv...))
    {
        if(!lib_) throw std::runtime_error("Library does not load or found");
        std::function<F> fn = (F*) ::dlsym(lib_, f_name.c_str());
        if(!fn) throw std::runtime_error("Function does not found");
        return fn(argv ...);
    }

private:
    void * lib_ = nullptr;
    std::string lib_path_;
    std::map<eFunction, std::string> f_name_list_ = {{kSetParameters, "PBLibrary_SetParameters"}, 
                                                     {kRun, "PBLibrary_Run"}, 
                                                     {kGetStatus, "PBLibrary_GetStatus"},
                                                     {kGetResult, "PBLibrary_GetResult"},
                                                     {kSetNotificationFlag, "PBLibrary_SetNotificationFlag"},
                                                     {kAddNotification, "PBLibrary_AddNotification"}};
    std::map<eFunction, void *>  f_pointer_list_;
};

} // namespace utilities
} // namespace pocketbook
