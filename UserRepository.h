#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H
#include "Connection.h"
#include "Request.h"
#include "User.h"
#include <memory>
#include <string>
class UserRepository
{
private:
	std::shared_ptr<Connections> connection_;
	std::string buildSelectQuery(const std::string& table,const std::string& condition);

	std::shared_ptr<User> findUserByField(const std::string& field, const std::string& value);


public:
	UserRepository(std::shared_ptr<Connections> conn);
	std::shared_ptr<User> findUserByLogin(const std::string& login);
	std::shared_ptr<User> findUserByEmail(const std::string& email);
	bool saveUser(std::shared_ptr<User>user);
	bool updateUser();// потом


};





#endif // !USERREPOSITORY_H

