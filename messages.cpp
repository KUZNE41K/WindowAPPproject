#include "messages.h"

Messages::Messages() : id_(0), threadId_(""), authorId_(0), content_(""), createdAt_("")
{
}

Messages::Messages(int id, const std::string& threadId, const int& authorId, const std::string& content, const std::string& createdAt) : id_(id), threadId_(threadId),
authorId_(authorId), content_(content), createdAt_(createdAt)
{
}


int Messages::getId() const
{
	return id_;
}

std::string Messages::getThreadId() const
{
	return threadId_;
}

int Messages::getAuthorId() const
{
	return authorId_;
}

std::string Messages::getContent() const
{
	return content_;
}

std::string Messages::getCreatedAt() const
{
	return createdAt_;
}

int Messages::setId(int id)
{
	return this->id_ = id;
}

std::string Messages::setThreadId(const std::string& threadId)
{
	return this->threadId_ = threadId;
}

int Messages::setAuthorId(int authorId)
{
	return this->authorId_ = authorId;
}

std::string Messages::setContent(const std::string& content)
{
	return this->content_ = content;
}

std::string Messages::setCreatedAt(const std::string& createdAt)
{
	return this->createdAt_ = createdAt;
}
