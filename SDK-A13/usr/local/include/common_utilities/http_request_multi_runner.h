#ifndef HTTP_REQUEST_MULTI_RUNNER_H
#define HTTP_REQUEST_MULTI_RUNNER_H

#include <functional>
#include <vector>
#include <curl/curl.h>
#include "http_request.h"
#include <map>
#include <memory>

namespace pocketbook
{
namespace utilities
{

// Should be used only from main inview thread
class HttpRequestMultiRunner
{
private:
    HttpRequestMultiRunner();
    HttpRequestMultiRunner(const HttpRequestMultiRunner&) = delete;
    HttpRequestMultiRunner& operator=(const HttpRequestMultiRunner&) = delete;
public:

    struct HandleInfo {
        HandleInfo(const HttpRequest& req):request_(req){}
        std::vector<char> received_data_;
        HttpRequestRunner::CurlAdditionalInfo additional_info_;
        HttpRequest request_;
        std::function<void(int, long, std::vector<char>&, HttpRequestRunner::HeaderRecords&)> cb_; // params are: err, http code, received data
    };

    static HttpRequestMultiRunner& instance();

    int PerformRequest(const HttpRequest &req, std::function<void(int, long, std::vector<char>&)>);
    int PerformRequestEx(const HttpRequest &req, std::function<void(int, long, std::vector<char>&, HttpRequestRunner::HeaderRecords&)>);

private:
    static int UsleepHandler(__useconds_t __useconds);
    int processTransfers();
    int processFinishedTransfer(CURLMsg *msg);
    static void timerHandler(void* context);
    void setProcessTimer();
    CURLM* multi_handle_;
    HttpRequestRunner request_runner_;
    std::map<CURL*, std::shared_ptr<HandleInfo> > handles_info_;
};

}
}
#endif // HTTP_REQUEST_MULTI_RUNNER_H
