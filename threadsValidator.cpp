#include "threadsValidator.h"

bool ThreadsValidator::validateCreate(const std::string& title, const std::string& uuid, int& createdId,std::string& errorMessage)
{
    if (title.empty() || uuid.empty() || createdId != 0 )
    {
		errorMessage = "Empty lines in the query";
		return false;
    }
    return true;
}
