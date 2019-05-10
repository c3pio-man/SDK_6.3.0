#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <functional>
#include <curl/curl.h>

namespace pocketbook
{
namespace utilities
{

class Url {
public:
    Url(const std::string& url);

    const std::string& protocol() const { return protocol_; }
    const std::string& host() const { return host_; }
    const std::string& path() const { return path_; }
    const std::string& query() const { return query_; }
    const std::string& port() const { return port_; }

    static std::map<std::string, std::string> ParseQuery(const std::string& query);
private:
    int Parse(const std::string& url);

    std::string protocol_, host_, port_, path_, query_;
};


class HttpRequest
{
private:
    std::string method_;
    std::string host_;
    std::string rel_uri_;
    std::string port_;
    std::string url_;
    std::string cookie_;
    std::map<std::string, std::string> headers_;
    std::string post_data_;
    bool disable_sert_validation_ = false;
    std::string raw_url_;
    std::string cookie_file_;
public:

    typedef std::vector<std::pair<std::string, std::string> > TParams;
    TParams params_;
    TParams post_params_;

    const std::string& method() const { return method_; }
    const std::string& host() const { return host_; }
    const std::string& rel_uri() const { return rel_uri_;}
    const std::string& port() const { return port_; }
    std::string url() const;
    std::string encoded_url() const;

    HttpRequest(const std::string& method, const std::string& host, const std::string& rel_uri, const std::string& port);

    HttpRequest(const std::string& method, const std::string& url);
    void SetRawUrl(const std::string url) { raw_url_ = url; }
    void SetUrl(const std::string &url_s);

    void AddGetParam(const std::string& name, const std::string& value);
    void AddGetParam(const std::string& name, int value);
    void AddGetParam(const std::string& name, double value);

    void AddPostParam(const std::string& name, const std::string& value);
    void AddPostParam(const std::string& name, int value);
    void AddPostParam(const std::string& name, double value);
    void AddPostData(const std::string& data);


    void SetCookie(const std::string& name, const std::string& value);
    std::string GetCookie() const {return cookie_; }
    std::string GetPostQuery() const;
    void AddHeader(const std::string& name, const std::string& value);
    std::vector<std::string> GetHeaders() const;
    void SetDisableSertValidation(bool value) { disable_sert_validation_ = value; }
    void EnableCookie(bool flag, const std::string &file = "");
    std::string GetCokieFile() const {return cookie_file_;}
private:
    void addParam(const std::string& name, const std::string& value, bool isGetParam);
    void buildQuery(std::stringstream *stream, const TParams & params) const;
    friend class HttpRequestRunner;
};

class HttpRequestRunner
{
public:
    typedef std::unordered_map<std::string,std::string> HeaderRecords;
    typedef std::function<size_t(size_t, char*)> BodyHandler;

    HttpRequestRunner():use_redirect_(true) {}
    ~HttpRequestRunner();
    void ForbidRedirect();
    void ReuseCurlConnection() { reuse_curl_connection_ = true; }
    struct CurlAdditionalInfo {
        curl_slist* slist = nullptr;
        std::string postQuery;
        ~CurlAdditionalInfo() { curl_slist_free_all(slist); }
    };

    int PrepareRequest(const HttpRequest &req, CURL* curl, std::vector<char> *received_data, CurlAdditionalInfo* info);

    int PerformRequest(const HttpRequest &req, long* response_code, std::vector<char>* received_data);
    int PerformRequest(const HttpRequest &req, long* response_code, HeaderRecords *hr, BodyHandler data_to_send, BodyHandler data_to_receive);
private:
    bool use_redirect_;
    bool reuse_curl_connection_ = false;
    void* curl_ = nullptr;

    static  size_t write_header( char *ptr, size_t size, size_t nmemb, void *userdata);
    static  size_t data_cb( char *ptr, size_t size, size_t nmemb, void *userdata);
};

} // namespace utilities
} // namespace pocketbook



#endif // HTTP_REQUEST_H
