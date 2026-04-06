#ifndef HASH_H
#define HASH_H

#include <string>
#include <argon2.h>
#include <random>
#include <vector>

class Hash
{
public:
	static std::string hash(const std::string& password);

	static bool verifyPassword(const std::string& password, const std::string& hash);

};


#endif // !HASH_H
