#ifndef THREADSREPOSITORY_H
#define THREADSREPOSITORY_H
#include <memory>
#include "Connection.h"
#include "threads.h"



class ThreadRepository
{
public:
	ThreadRepository(std::shared_ptr<Connections> conn);
	bool createThread(const std::string& title, const int& creatorId, const std::string& uuid);
	bool deleteThread(const std::string& threadId);
	bool updateThread(const std::string& threadId, const std::string& newTitle);
	bool userSearch(const int& creatorId);

private:
	std::shared_ptr<Connections> connection_;

};
#endif // !THREADSREPOSITORY_H

