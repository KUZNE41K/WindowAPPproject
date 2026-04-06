#ifndef PASSWORDCHECKHANDLER_H
#define PASSWORDCHECKHANDLER_H
#include "Hash.h"
#include "Handler.h"
#include "Request.h"
#include "User.h"
#include <string>



class PasswordCheckHandler : public Handler
{
public:
	PasswordCheckHandler();
	void handle(Request& request) override;

private:
	bool checkPassword(const std::string& password, const std::string& hash);

};
#endif // !PASSWORDCHECKHANDLER_H

