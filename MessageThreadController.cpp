#include "MessageThreadController.h"


MessageThreadController::MessageThreadController(std::shared_ptr<MessagesRepository> messageThreadsRepo) : messageThreadsRepository_(messageThreadsRepo)
{

}

bool MessageThreadController::createMessageThread(std::string threadId, int& userId, std::string& content)
{
    if (!messageThreadsRepository_ -> createMessage(threadId, userId, content))
    {
        return false;
    }
    return true;
}

bool MessageThreadController::deleteMessageThread(std::string threadId, int& messageId)
{
    if (!messageThreadsRepository_ -> deleteMessage(threadId,messageId))
    {
        return false;
    }
    return true;
    
}

bool MessageThreadController::updateContentMessageThread(std::string threadId, int& messageId, const std::string& newContent)
{
    if (!messageThreadsRepository_ -> updateMessage(threadId, messageId,newContent))
    {
        return false;
    }
    return true;
    
}
