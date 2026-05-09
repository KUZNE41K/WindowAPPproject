#include <string>
#ifndef MESSAGES_THREAD_VALIDATOR_H
#define MESSAGES_THREAD_VALIDATOR_H


class MessagesThreadValidator
{
public:
	static bool validateCreate(std::string threadId, int& userId, std::string& content, std::string& errorMessage);
	static bool validateDelete(std::string threadId, int& messageId, std::string& errorMessage);
	static bool validateUpdateContent(std::string threadId, int& messageId, const std::string& newContent, std::string& errorMessage);




};




#endif // !MESSAGES_THREAD_VALIDATOR_H
