#ifndef USERFETCHHANDLER_H
#define USERFETCHHANDLER_H
#include "Handler.h"
#include <string>

class UserRepository;

class UserFetchHandler : public Handler
{
private:
	std::shared_ptr<UserRepository> userRepository_;
public:
	explicit UserFetchHandler(std::shared_ptr<UserRepository> userRepository);
	~UserFetchHandler() override = default;
	void handle(Request& request) override; // Ётот метод будет извлекать 
	//пользовател€ из базы данных на основе логина или электронной почты, предоставленных в запросе.
	// ≈сли пользователь найден, он будет установлен в объекте запроса. ≈сли пользователь не найден, будет установлено сообщение об ошибке.


};




#endif // !USERFETCHHANDLER_H

