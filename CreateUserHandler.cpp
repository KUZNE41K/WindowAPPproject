#include "CreateUserHandler.h"
#include <iostream>

CreateUserHandler::CreateUserHandler(std::shared_ptr<UserRepository> repo) : repo_(repo)
{
}

void CreateUserHandler::handle(Request& request)
{
    std::cout << "=== CreateUserHandler::handle ===" << std::endl;
    std::cout << "request.user_ is " << (request.user_ == nullptr ? "null" : "not null") << std::endl;

    if (request.user_ != nullptr)
    {
        std::cout << "User already exists, failing" << std::endl;
        request.setSuccess(false);
        request.setErrorMessage("User already exists.");
        return;
    }

    std::cout << "Calling createUser..." << std::endl;
    createUser(request);

    std::cout << "After createUser, success = " << request.success_ << std::endl;

    if (next_ && request.success_)
    {
        std::cout << "Calling next handler" << std::endl;
        next_->handle(request);
    }
}

void CreateUserHandler::createUser(Request& request)
{
    std::cout << "=== CreateUserHandler::createUser ===" << std::endl;
    std::cout << "Login: " << request.login_ << std::endl;
    std::cout << "Email: " << request.email_ << std::endl;
    std::cout << "Password length: " << request.password_.size() << std::endl;

    std::cout << "Calling Hash::hashPassword..." << std::endl;
    std::string passwordHash = Hash::hashPassword(request.password_);
    std::cout << "Password hash generated: " << passwordHash.substr(0, 30) << "..." << std::endl;

    auto userPtr = std::make_shared<User>();
    userPtr->setLogin(request.login_);
    userPtr->setEmail(request.email_);
    userPtr->setPasswordHash(passwordHash);

    std::cout << "Calling repo_->saveUser..." << std::endl;
    if (!repo_->saveUser(userPtr))
    {
        std::cout << "repo_->saveUser returned FALSE!" << std::endl;
        request.setSuccess(false);
        request.setErrorMessage("Failed to create user.");
        return;
    }

    std::cout << "repo_->saveUser returned TRUE" << std::endl;
    request.user_ = userPtr;
    request.setSuccess(true);
    request.setErrorMessage("");
    std::cout << "User created successfully!" << std::endl;
}