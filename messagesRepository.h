#ifndef MESSAGESREPOSITORY_H
#define MESSAGESREPOSITORY_H
#include <memory>
#include "Connection.h"
#include "messages.h"

class MessagesRepository
{
public:
    MessagesRepository(std::shared_ptr<Connections> conn);
    bool createMessage(std::shared_ptr<Messages> message);
    bool updateMessage(int& messageId, const std::string& newContent);

private:
    std::shared_ptr<Connections> connection_;
};

#endif // !MESSAGESREPOSITORY_H
