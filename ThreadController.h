#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H
#include "threadsRepository.h"
#include "threads.h"

class ThreadRepository;
class Threads;

class ThreadController
{
public:
	ThreadController(std::shared_ptr<ThreadRepository> userRepo);

	bool createThread(const std::string& title, int& createdId, const std::string& uuid);
	bool deleteThread();
	bool updateTitleThread();

	
private:
	std::shared_ptr<ThreadRepository> threadsRepository_;
	std::shared_ptr<Threads> threads_;
};

#endif // !THREADCONTROLLER_H

