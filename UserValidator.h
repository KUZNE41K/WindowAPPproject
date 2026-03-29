#ifndef USERVALIDATOR_H
#define USERVALIDATOR_H
#include<iostream>
#include<string>
#include"User.h"

class UserValidator
{
public:
	static bool identification(const User& user, const std::string& login);// проверка существования пользователя для входа
	// static bool registration(const User& user, const std::string& login, const std::string& email, const std::string& password); // проверка на уникальность логина при регистрации и
	// далее регестрация пользователя
	// сделать через наследование от класса userValidator
	// переписатьь под разные классы для регистрации и для входа
};



#endif // USERVALIDATOR_H


