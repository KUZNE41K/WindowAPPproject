#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <memory>
#include <string>
#include "threadsRepository.h"
#include <nlohmann/json.hpp>

class ThreadController
{
public:
	ThreadController(std::shared_ptr<ThreadRepository> threadsRepo);
	bool createThread(const std::string& title, int creatorId, const std::string& uuid);
	bool deleteThread(const std::string& threadId);
	bool updateTitleThread(const std::string& threadId, const std::string& newTitle);

	nlohmann::json getTabs(int& userId);

private:
	std::shared_ptr<ThreadRepository> threadsRepository_;
	std::shared_ptr<Threads> threads_;
};

#endif // !THREADCONTROLLER_H

