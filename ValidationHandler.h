#ifndef VALIDATIONHANDLER_H
#define VALIDATIONHANDLER_H
#include "Handler.h"
#include <string>


class ValidationHandler : public Handler
{
public:
	ValidationHandler() = default;
	~ValidationHandler() = default;

	void handle(Request& request) override
	{
		if (request.login_.empty() || request.password_.empty())
		{
			request.setErrorMessage("Login and password cannot be empty.");
			request.success_ = false;
			return;
		}
		Handler::handle(request);
	}



};


#endif // !VALIDATIONHANDLER_H

