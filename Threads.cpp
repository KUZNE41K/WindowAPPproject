#include "Threads.h"

Threads::Threads() : title_(""), uuid_(""), createdId_("")
{
}

Threads::Threads(const std::string& title, const std::string& uuid, const std::string& createdId) : title_(title), uuid_(uuid), createdId_(createdId)
{
}

std::string Threads::getTitle() const
{
	return title_;
}

std::string Threads::getUuid() const
{
	return uuid_;
}

std::string Threads::getCreatedId() const
{
	return createdId_;
}

void Threads::setTitle(const std::string& title)
{
	this->title_ = title;
}

void Threads::setUuid(const std::string& uuid)
{
	this->uuid_ = uuid;
}

void Threads::setCreatedId(const std::string& createdId)
{
	this->createdId_ = createdId;
}
