#ifndef MESSAGESREPOSITORY_H
#define MESSAGESREPOSITORY_H
#include <memory>
#include "Connection.h"
#include "messages.h"

class MessagesRepository
{
public:
    MessagesRepository(std::shared_ptr<Connections> conn);
    bool createMessage(std::string threadId,int& userId,std::string& content);
    bool updateMessage(std::string threadId, int& messageId, const std::string& newContent);
    bool deleteMessage(std::string threadId, int& messageId);

private:
    std::shared_ptr<Connections> connection_;
};

#endif // !MESSAGESREPOSITORY_H
