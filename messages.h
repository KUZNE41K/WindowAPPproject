#include <string>
#ifndef MESSAGES_H
#define MESSAGES_H


class Messages
{
	public:
	Messages();
	Messages(int id, const std::string& threadId, const int& authorId, const std::string& content, const std::string& createdAt);

	int getId() const;
	std::string getThreadId() const;
	int getAuthorId() const;
	std::string getContent() const;
	std::string getCreatedAt() const;

	int setId(int id);
	std::string setThreadId(const std::string& threadId);
	int setAuthorId(int authorId);
	std::string setContent(const std::string& content);
	std::string setCreatedAt(const std::string& createdAt);


private:
	int id_;
	std::string threadId_;
	int authorId_;
	std::string content_;
	std::string createdAt_;
};
#endif // !MESSAGES_H

