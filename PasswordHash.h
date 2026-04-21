#ifndef PASSWORD_HASH_H
#define PASSWORD_HASH_H

#include <string>
#include <argon2.h>
#include <random>
#include <vector>

class PasswordHash
{
public:
	static std::string hashPassword(const std::string& password);
	static std::string hashToken(const std::string& token);



	static std::string hash(const std::string& input, uint32_t t_cost, uint32_t m_cost);

	static bool verifyPassword(const std::string& password, const std::string& hash);

};


#endif // !PASSWOR_HASH_H
