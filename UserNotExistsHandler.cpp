#include "UserNotExistsHandler.h"

void UserNotExistsHandler::handle(Request& request)
{
	if(request.user_ != nullptr)
	{
		request.setSuccess(false);
		request.setErrorMessage("User already exists.");
		return;
	}
	request.success_ = true;
	request.setErrorMessage("");
	if(next_ != nullptr)
	{
		next_->handle(request);
	}
}
