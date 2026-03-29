#ifndef USERVALIDATOR_H
#define USERVALIDATOR_H

#include<string>
#include"User.h"

class UserValidator
{
public:
	static bool authenticate(const User& user, const std::string& login, const std::string& password);
};



#endif // USERVALIDATOR_H


