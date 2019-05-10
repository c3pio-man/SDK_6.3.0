// Created by Denis Kuzko

#ifndef OSYUTILITIES_H
#define OSYUTILITIES_H

#include <string>
#include <openssl/sha.h>


#define KEY_MAX_SIZE 256
class OsyUtilities
{
public:
        OsyUtilities();

	static const char *SHA1Crypt(const char *data, int size);

	// Use for find client secret, don't use SHA algorythm.
	static const char *SHA0BadCrypt();
    static void SetSha0Data(const char* data);

	static std::string QuickMD5(const std::string &path);
	// crypt-uncrypt data
	// start_num - number of start search prime number.
	static void get_data(int start_num, const char *indata, char *outdata, int size, int crypt);

private:
    static char m_hash[40];
    static char m_hash0[KEY_MAX_SIZE];
};

#endif // OSYUTILITIES_H
