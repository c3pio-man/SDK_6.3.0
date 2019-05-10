#include <time.h>
#include <string>
namespace pocketbook
{
namespace utilities
{
std::string ToLower(const std::string & text);
std::string ToUpper(const std::string & text);
class HumanStringUtility {
public:
static int HumanSize(char *buf, unsigned long long bsize, unsigned long long nblocks);
static int HumanDate(char *buf, time_t t);
};
}
}
