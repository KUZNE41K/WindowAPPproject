#ifndef USER_H
#define USER_H
#include <string>

class User
{
private:
	int id_;
	std::string email_;
	std::string login_;
	std::string passwordHash_;
public:
	User();
	User(int id, const std::string& login,const std::string& email, const std::string& passwordHash);

	int getId() const;
	std::string getEmail() const;
	std::string getLogin() const;
	std::string getPasswordHash() const;

	int setId(int id);
	std::string setEmail(const std::string& email);
	std::string setLogin(const std::string& login);
	std::string setPasswordHash(const std::string& passwordHash);

	bool isValid() const; 
};
#endif // !USER_H
