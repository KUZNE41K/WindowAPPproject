#pragma once
#include <string>

class User
{
private:
	int id_;
	std::string login_;
	std::string passwordHash_;
public:
	User();
	User(int id, const std::string& login, const std::string& passwordHash, const std::string& salt);

	int getId() const;
	std::string getLogin() const;
	std::string getPasswordHash() const;

	int setId(int id);
	std::string setLogin(const std::string& login);
	std::string setPasswordHash(const std::string& passwordHash);





};
