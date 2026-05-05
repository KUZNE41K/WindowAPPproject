#include "threadsValidator.h"

bool ThreadsValidator::validateCreate(const std::string& title, const std::string& uuid, const std::string& createdId,std::string& errorMessage)
{
    if (title.empty() || uuid.empty() || createdId.empty())
    {
		errorMessage = "Empty lines in the query";
		return false;
    }
}
