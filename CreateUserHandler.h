#ifndef CREATEUSERHANDLER_H
#define CREATEUSERHANDLER_H
#include "Handler.h"
#include "Request.h"
#include "User.h"
#include "UserRepository.h"
#include "Hash.h"

class CreateUserHandler : public Handler
{
	explicit CreateUserHandler(UserRepository& repo);

	void handle(Request& request) override; // Этот метод будет создавать нового пользователя на основе данных, 
	//предоставленных в объекте Request, и сохранять его в базе данных с помощью UserRepository. 
	// Если сохранение успешно, он будет устанавливать флаг успеха в объекте Request. Если сохранение не удалось, он будет устанавливать сообщение об ошибке.
private:
	UserRepository& repo_;
	void createUser(Request& request);
};


#endif // !CREATEUSERHANDLER_H

