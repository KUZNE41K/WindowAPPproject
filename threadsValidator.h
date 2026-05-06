#include <string>
#ifndef THREADSVALIDATOR_H
#define THREADSVALIDATOR_H

class ThreadsValidator {
public:
    static bool validateCreate(const std::string& title, const std::string& uuid, int& createdId, std::string& errorMessage);
};

#endif // THREADSVALIDATOR_H
