#ifndef THREADSREPOSITORY_H
#define THREADSREPOSITORY_H
#include <memory>
#include "Connection.h"
#include "threads.h"



class ThreadRepository
{
public:
	ThreadRepository(std::shared_ptr<Connections> conn);
	bool createThread(const std::string& title, const int& createdId, const std::string& uuid);
	bool deleteThread(int& threadId);
	bool updateThread(int& threadId, const std::string& newTitle);
	bool userSearch(int& creatorId);

private:
	std::shared_ptr<Connections> connection_;

};
#endif // !THREADSREPOSITORY_H

