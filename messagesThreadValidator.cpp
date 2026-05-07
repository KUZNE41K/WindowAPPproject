#include "messagesThreadValidator.h"

bool MessagesThreadValidator::validateCreate(std::string threadId, int& userId, std::string& content, std::string& errorMessage)
{
    if (threadId.empty() || userId<= 0 || content.empty())
    {
        errorMessage = "Empty lines in the query";
        return false;
    }
    return true;
}

bool MessagesThreadValidator::validateDelete(std::string threadId, int& messageId, std::string& errorMessage)
{
    if (threadId.empty() || messageId <= 0)
    {
        errorMessage = "Empty lines in the query";
        return false;
    }
    return true;
}

bool MessagesThreadValidator::validateUpdateContent(std::string threadId, int& messageId, const std::string& newContent, std::string& errorMessage)
{
    if (threadId.empty() || messageId <= 0 || newContent.empty())
    {
        errorMessage = "Empty lines in the query";
        return false;
    }
    return true;
}
