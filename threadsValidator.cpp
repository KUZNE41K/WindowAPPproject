#include "threadsValidator.h"

bool ThreadsValidator::validateCreate(const std::string& title, const std::string& uuid, int& createdId,std::string& errorMessage)
{
    if (title.empty() || uuid.empty() || createdId <= 0 )
    {
		errorMessage = "Empty lines in the query";
		return false;
    }
    return true;
}

bool ThreadsValidator::validateDelete(std::string& threadId, std::string& errorMessage)
{
    if (threadId.empty())
    {
        errorMessage = "Invalid thread ID";
        return false;
    }
	return true;
    
}

bool ThreadsValidator::validateUpdateRename(const std::string& threadId, const std::string& newTitle, std::string& errorMessage)
{
    if (threadId.empty() || newTitle.empty())
    {
		errorMessage = "Thread ID and new title must not be empty";
		return false;
    }
	return true;
}
