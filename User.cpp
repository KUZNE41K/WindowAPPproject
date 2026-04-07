#include "User.h"

User::User(): id_(0), login_(""),email_(""), passwordHash_("")
{
}

User::User(int id, const std::string& login,const std::string& email, const std::string& passwordHash): id_(id), login_(login),email_(email), passwordHash_(passwordHash)
{
}

int User::getId() const
{
	return id_;
}
std::string User::getEmail() const
{
	return email_;
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
	return this->id_ = id;
}
std::string User::setEmail(const std::string& email)
{
	return this ->email_ = email;
}
std::string User::setLogin(const std::string& login)
{
	return this->login_ = login;
}
std::string User::setPasswordHash(const std::string& passwordHash)
{
	return this->passwordHash_ = passwordHash;
}

bool User::isValid() const
{
	return id_ != 0 && !login_.empty();
}



