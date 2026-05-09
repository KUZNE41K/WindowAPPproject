#include "ThreadController.h"
#include "Connection.h"

ThreadController::ThreadController(std::shared_ptr<ThreadRepository> threadsRepo) : threadsRepository_(threadsRepo)
{
}

bool ThreadController::createThread(const std::string& title, int creatorId, const std::string& uuid)
{
	// проверяем, существует ли пользователь-автор
	if (!threadsRepository_->userSearch(creatorId))
	{
		return false;
	}
	return threadsRepository_->createThread(title, creatorId, uuid);
}

bool ThreadController::deleteThread(const std::string& threadId)
{
	if (!threadsRepository_->deleteThread(threadId))
	{
		return false;
	}
	return true;
}

bool ThreadController::updateTitleThread(const std::string& threadId, const std::string& newTitle)
{
	if (!threadsRepository_->updateThread(threadId, newTitle))
	{
		return false;
	}
	return true;
}
