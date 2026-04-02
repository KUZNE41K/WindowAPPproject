#include "User.h"

User::User(): id_(0), login_(""), passwordHash_("")
{
}

User::User(int id, const std::string& login, const std::string& passwordHash, const std::string& salt): id_(id), login_(login), passwordHash_(passwordHash)
{
}

int User::getId() const
{
	return id_;
}
std::string User::getLogin() const
{
	return login_;
}
std::string User::getPasswordHash() const
{
	return passwordHash_;
}

int User::setId(int id)
{
	this->id_ = id;
}
std::string User::setLogin(const std::string& login)
{
	this->login_ = login;
}
std::string User::setPasswordHash(const std::string& passwordHash)
{
	this->passwordHash_ = passwordHash;
}



