#ifndef USERNOTEXISTHANDLER_H
#define USERNOTEXISTHANDLER_H
#include "Handler.h"
#include "Request.h"

class UserNotExistsHandler : public Handler
{	
	public:
	UserNotExistsHandler() = default;
	~UserNotExistsHandler() override = default;
	void handle(Request& request) override; // Этот метод будет проверять, существует ли пользователь, извлеченный UserFetchHandler, в базе данных.
	// Если пользователь существует, будет установлено сообщение об ошибке.
	// Если пользователь не существует, он будет передан следующему обработчику в цепочке.
};

#endif // !USERNOTEXISTHANDLER_H
