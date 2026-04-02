#include "UserExistsHandler.h"

void UserExistsHandler::handle(Request& request)
{
	if(request.user_ == nullptr)
	{
		request.setSuccess(false);
		request.setErrorMessage("User does not exist.");
		return;
	}

	if(next_ != nullptr)
	{
		next_->handle(request);
	}
	
	
}
