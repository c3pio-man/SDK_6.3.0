#ifndef WIFI_H
#define WIFI_H

#include <vector>
#include <string>
#include <map>
#include <inkview.h>
#include <memory>

#include <common_utilities/json_parser_utils.h>
#include "wifi_async_wrapper.h"


namespace pocketbook {
namespace system {

struct ProxyParam {
    int proxyconfig; // 0 - off, 1 - manual
    std::string host;
    int port;
    std::string user;
    std::string pass;

    ProxyParam():proxyconfig(0),port(0){}
};

struct NetworkParam {
    NetworkParam():ipconfig(0){}
    std::vector<std::string > dns_list;
    std::string ip;
    int ipconfig; // 0 - dhcp, 1 - manual
    std::string mask;
    std::string gateway;
    ProxyParam proxy;
};

struct WifiInfo{
    WifiInfo():hidden(0), idle(10), mode(0), security(NO), wepmode(0),index_in_file(-1){tech_spec.level = 1;}
    WifiInfo(const wifiapinfo& ap_info):hidden(0), idle(10), mode(0), security(NO), wepmode(0),index_in_file(-1),tech_spec(ap_info)
    {
        essid = tech_spec.ssid;
        mode = tech_spec.mode;
        security = (WIFI_SECURITY)tech_spec.security;
    }

    void UpdateTechSpec()   // Update tech spec fields based on member fields
    {
        snprintf(tech_spec.ssid, sizeof(tech_spec.ssid), "%s", essid.c_str());
        tech_spec.mode = mode;
        tech_spec.security = security;
    }

    void UpdateConfiguration(const WifiInfo& wifi) {
        essid = wifi.essid;
        cacert = wifi.cacert;
        hidden = wifi.hidden;
        idle = wifi.idle;
        key = wifi.key;
        mode = wifi.mode;
        security = wifi.security;
        wepmode = wifi.wepmode;
        eaptype = wifi.eaptype;
        username = wifi.username;
        usercert = wifi.usercert;
        password = wifi.password;
        index_in_file = wifi.index_in_file;
        network_param = wifi.network_param;
    }

    std::string essid; // network identifier
    std::string cacert;
    int hidden; // does network scannable
    int idle;   // time in minutes to shutdown interface if we have not any activity
    std::string key;    //
    int mode;   // 1 - AP , 4 - ad-hoc
    WIFI_SECURITY security; // value from WIFI_SECURITY
    int wepmode;
    std::string eaptype;
    std::string username;
    std::string usercert;
    std::string password;
    std::string domain;
    int index_in_file;
    NetworkParam network_param;

    wifiapinfo tech_spec;
};

class Network;

std::string DecryptString(const std::string &str);
json_object* ProxyObject(const ProxyParam& proxy);
json_object* NetworkParamObject(const NetworkParam& network_param);

class NetworkInfo
{
public:
    enum NetworkType {
        kWifi,
        k3G,
        kBlueTooth,
    };
    NetworkInfo() : id_(""), ssid_(""), level_(0), is_connected_(0), security_(NO), quality_(0),noise_(0), is_connecting_(false){}
    NetworkInfo(const wifiapinfo& wifi_info, bool is_connected, int index_in_file, bool is_connecting);
    
    NetworkType type() const { return type_;}
    const std::string& id() const { return id_; }
    const std::string& ssid() const { return ssid_; }
    int level() const {return level_; }
    bool is_connected() const {return is_connected_; }
    WIFI_SECURITY security() const { return security_; }
    int quality() const { return quality_; }
    int noise() const { return noise_; }
    bool is_connecting() const {return is_connecting_; }

    friend class Network;
private:
    NetworkType type_;
    std::string id_;    // Unique network id (now it is ssid + mac)
    std::string ssid_;
    int level_;
    bool is_connected_;
    WIFI_SECURITY security_;
    int quality_;
    int noise_;
    bool is_connecting_;
};

class DetailedNetworkInfo
{
public:
    DetailedNetworkInfo(){}
    DetailedNetworkInfo(const WifiInfo& wifi_info):wifi_info_(wifi_info){}
    enum ValidationErrors
    {
        kInvalid,
    };

    ValidationErrors Validate();
    std::string ssid() const { return wifi_info_.essid; }
    void SetSsid(const std::string& ssid) { wifi_info_.essid = ssid; }
    std::string cacert() { return wifi_info_.cacert; }
    void SetCacert(const std::string cacert) { wifi_info_.cacert = cacert; }
    bool hidden() { return wifi_info_.hidden; }
    void SetHidden(bool is_hidden) { wifi_info_.hidden = is_hidden; }
    int idle() { return wifi_info_.idle; }
    void SetIdle(int idle) { wifi_info_.idle = idle; }
    std::string key() { return wifi_info_.key; }
    void SetKey(const std::string& key) { wifi_info_.key = key; }
    int mode() { return wifi_info_.mode; }
    void SetMode(int mode) { wifi_info_.mode = mode; }
    WIFI_SECURITY security() { return wifi_info_.security; }
    void SetSecurity(WIFI_SECURITY security) { wifi_info_.security = security; }
    int wepmode() { return wifi_info_.wepmode; }
    void SetWepMode(int wepmode) { wifi_info_.wepmode = wepmode; }
    std::string eaptype() { return wifi_info_.eaptype; }
    void SetEaptype(const std::string& eaptype) { wifi_info_.eaptype = eaptype; }
    std::string username() { return wifi_info_.username; }
    void SetUserName(const std::string& username) { wifi_info_.username = username; }
    std::string usercert() { return wifi_info_.usercert; }
    void SetUserCert(const std::string& usercert) { wifi_info_.usercert = usercert; }
    std::string password() { return wifi_info_.password; }
    void SetPassword(const std::string& password) { wifi_info_.password = password; }
    std::string domain() { return wifi_info_.domain; }
    void SetDomain(const std::string& domain) { wifi_info_.domain = domain; }

    const NetworkParam& network_param() { return wifi_info_.network_param; }
    void SetNetworkParam(const NetworkParam& network_param) { wifi_info_.network_param = network_param; }
    friend class Network;

    /**
      \brief returns non empty parameters only if network is connected
      */
    NetworkParam GetConnectedNetParam();
private:
    WifiInfo wifi_info_;
};

class Network
{
public:
    enum SortOrder{
        kSortByLevel,
        kSortByQuality,
    };

    ~Network();

    static Network& GetInstance();

    enum CallbackTypes {
        kCbSwitchNetworkManager = WifiAsyncWrapper::kCbSwitchNetworkManager,
        kCbAdd = WifiAsyncWrapper::kCbAdd,
        kCbSelect = WifiAsyncWrapper::kCbSelect,
    };

    typedef WifiAsyncWrapper::Cb Cb;
    /**
      \brief Get list of wifi connection points found by scanner (including configured hidden networks)
      */
    std::vector<NetworkInfo> GetWifiList();

    /**
      \brief Get sorted list of wifi connection points found by scanner (including configured hidden networks)
      */
    std::vector<NetworkInfo> GetSortedWifiList(SortOrder sort_by);

    /**
      \brief Get detailed information about wifi connection point
      */
    DetailedNetworkInfo GetDetailedInfo(const NetworkInfo&);

    NetworkInfo SetDetailedInfo(DetailedNetworkInfo& info);
    NetworkInfo SetDetailedInfoToCache(const DetailedNetworkInfo& info);
    void RemoveDetailedInfoFromCache(const DetailedNetworkInfo& info);

    int SwitchNetworkManagerAsync(int status, Cb* cb);

    bool IsNetworkManagerTurnedOn();

    void PostponeTimedNetOff();
    /**
      \brief Set configuration for the network and connect. It would be automatically saved to configuration file.
      */
    int AddAndConnectAsync(const NetworkInfo& net_info, Cb* cb);

    /**
      \brief Select network for connection.
      */
    int SelectAsync(NetworkInfo&, Cb* cb);
    /**
      \brief Remove network configuration from configuration file
      */
    int ForgetNetwork(NetworkInfo&);

    /**
      \brief Disconnect wifi
      */
    int DisconnectWifi();

    /**
      \brief Check if active network connection present
      */
    static bool IsNetworkConnected();

    /**
      \brief turn wifi on/off
      */
    int WiFiPower(int status);

    void CancelCallback(Cb* cb);

    void ConnectAsyncFinished();

    void UpdateStoredNetworks();

    bool IsConfigured(const NetworkInfo&) const;
    bool IsConnecting() const {return !connecting_network_id_.empty();}
    int FillConfig(const NetworkInfo& net_info, const std::string& filename);
    int FillConfig(DetailedNetworkInfo& detailed_info, const std::string& filename);
private:
    void ScanForNetworks();
    void UpdateCurrentConnectionInfo();
    void SaveNetworks();
    int GetIndexById(const DetailedNetworkInfo&);

    static void OnTimer();
    std::vector<WifiInfo> GetKnownWifis();

private:
    Network();
    Network& operator=(const Network&);
    Network(const Network&);
    static std::shared_ptr<Network> s_pThis;
    std::map<std::string, WifiInfo> wifi_list_; // List of networks from scanner
    std::vector<WifiInfo> wifi_from_file_; // List of networks from configuration
    std::string current_wifi_connection_;   // net id of connected network
    bool is_scanning_;
    bool is_timer_;
    std::string connecting_network_id_;
#ifdef EMULATOR
    bool is_turned_on_ = true;
#endif
    int wifi_power_count_ = 0;
    std::map<std::string, WifiInfo> wifi_cache_;
};

class NetworkParser {
public:
    static std::vector<WifiInfo> ReadWiFiConfig(const std::string &json_file);
    static void SaveNetworks(const std::vector<WifiInfo> &networks, const std::string &json_file);
private:
    static int ParseWiFiConfig(utilities::JsonParser &parser);
    static ProxyParam ParseProxyParam(pocketbook::utilities::JsonParser &parser);
    static NetworkParam ParseNetworkParam(utilities::JsonParser &parser);
    
private:
    static std::vector<WifiInfo> networks_;
};

} // namespace system
} // namespace pocketbook

#endif // WIFI_H
