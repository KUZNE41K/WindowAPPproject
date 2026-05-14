#include <memory>
#include "messagesRepository.h"
#include <nlohmann/json.hpp>

#ifndef MESSAGE_THREAD_CONTROLLER_H
#define MESSAGE_THREAD_CONTROLLER_H

class MessageThreadController
{
public:
	MessageThreadController(std::shared_ptr<MessagesRepository> messageThreadsRepo);
	bool createMessageThread(std::string threadId, int& userId, std::string& content);
	bool deleteMessageThread(std::string threadId, int& messageId);
	bool updateContentMessageThread(std::string threadId, int& messageId, const std::string& newContent);

	nlohmann::json getMessage(const std::string& threadId);



private:
	std::shared_ptr<MessagesRepository> messageThreadsRepository_;
};




#endif // !MESSAGE_THREAD_CONTROLLER_H

