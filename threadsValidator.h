#include <string>
#ifndef THREADSVALIDATOR_H
#define THREADSVALIDATOR_H

class ThreadsValidator {
public:
    static bool validateCreate(const std::string& title, const std::string& uuid, int& createdId, std::string& errorMessage);
	static bool validateDelete(std::string& threadId, std::string& errorMessage);
	static bool validateUpdateRename(const std::string& threadId, const std::string& newTitle, std::string& errorMessage);
};

#endif // THREADSVALIDATOR_H
