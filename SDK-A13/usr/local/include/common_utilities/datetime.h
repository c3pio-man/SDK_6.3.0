#ifndef DATETIME_H
#define DATETIME_H

#include <string>

namespace pocketbook {
namespace system {
namespace datetime {

typedef struct tm s_time;
typedef long long int64;

/**
  * \brief Function return seconds from specified date-time
  * \param cur_time - date-time to calculate secconds since 1900
  * \return seccond from 1900 year to specified date-time
**/
time_t GetSeconds(const s_time *cur_time);
int64 GetMilliSeconds();

/**
  * \brief Function convert date-time struct to human string
  * \param time - specified date-time
  * \return human string of specified date-time (19/08/13 14:27)
**/
std::string GetStringDateTime(const s_time &time);
std::string GetStringDateTime(const time_t &time_in_seconds);
std::string GetStringDateTime(const std::string &time_in_seconds);

/**
  * \brief Function return current date-time
  * \return current date-time in secconds
**/
s_time GetCurrentDateTime();

/**
  * \brief Convert specified date-time to secconds in string format
  * \param cur_time - date-time to convert or NULL if need to use current date-time 
  * \return secconds in string format
**/
std::string GetStringSeconds(const s_time *cur_time);

/**
  * \brief Function check the specified date-time
  * \param ctm - entered date-time
  * \return true if date-time is correct or false in otherwise
**/
bool ValidateDate(const s_time &ctm);

timespec GetTimeoutAbsTime(int timeout_ms);

class Trace {
public:
    static void StartTrace();
    static void StopTrace();
    static int64 GetTimeTrace();
    static int64 GetTimeFromLastTrace();
    static void PrintTraceInfo(const std::string &t_info = "");
private:
    static int64 start_time_;
    static int64 last_trace_time_;
    static int64 stop_trace_time_;
};
} // namespace datetime
} // namespace system
} // namespace pocketbook

#endif // DATETIME_H
