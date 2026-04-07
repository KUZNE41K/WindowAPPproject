#ifndef REQUEST_H
#define REQUEST_H
#include <string>
#include <memory>
#include "User.h"




class User;

// Класс Request представляет собой структуру данных, которая содержит информацию о запросе на регистрацию или аутентификацию пользователя. 
// Он включает в себя поля для хранения логина, электронной почты, пароля, соли, а также флаг успеха и сообщение об ошибке. Кроме того, он может 
// содержать указатель на объект User, который будет заполнен при успешной аутентификации.
class Request
{
public:
	std::string login_;
	std::string email_;
	std::string password_;
	std::string salt_;
	bool success_ = false;
	std::string errorMessage_;
	std::shared_ptr<User> user_;
	std::string sessionToken_;

	Request( const std::string& login,
		const std::string& email,
		const std::string& password
		) : login_(login), email_(email), password_(password),success_(false),errorMessage_(""),user_(nullptr),sessionToken_("") {
	}
	~Request() = default;

	void setSuccess(bool success);
	void setErrorMessage(const std::string& errorMessage);
	void setUser(std::shared_ptr<User> user);
	

};

#endif // REQUEST_H

