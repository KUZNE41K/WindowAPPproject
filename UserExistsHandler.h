#ifndef USEREXISTSHANDLER_H
#define USEREXISTSHANDLER_H
#include "Handler.h"
#include "Request.h"

class UserExistsHandler : public Handler
{
public:
	UserExistsHandler() = default;
	~UserExistsHandler() override = default;
	void handle(Request& request) override; // Ётот метод будет провер€ть, существует ли пользователь, извлеченный UserFetchHandler, в базе данных. 
	//≈сли пользователь существует, он будет установлен в объекте запроса. ≈сли пользователь не существует, будет установлено сообщение об ошибке.

};



#endif // !USEREXISTSHANDLER_H

