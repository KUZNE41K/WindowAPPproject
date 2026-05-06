#include "ThreadController.h"
#include "Connection.h"
ThreadController::ThreadController(std::shared_ptr<ThreadRepository> userRepo) : threadsRepository_(userRepo)
{
}
bool ThreadController::createThread(const std::string& title, int& createdId, const std::string& uuid)
{
	if (!threadsRepository_->userSearch(createdId))
	{
		return false;
	}
	threadsRepository_->createThread(title, createdId, uuid);
	return true;
}
