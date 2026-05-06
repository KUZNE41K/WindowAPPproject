#include "Router.h"
#include "AuthService.h"
#include "RequestParser.h"
#include <iostream>

Router::Router(
    std::shared_ptr<AuthService> authService, 
    std::shared_ptr<ThreadController> threadController) 
    : authService_(authService), 
    threadController_(threadController)
{
}

std::shared_ptr<Request> Router::route(const std::string& path, std::string& body)
{
    std::cout << "=== Router::route ===" << std::endl;
    std::cout << "Path: " << path << std::endl;
    std::cout << "Body: " << body << std::endl;

    auto request = RequestParser::parseRequest(body);


    std::cout << "After parsing, success: " << request->success_ << std::endl;
    if (!request->success_) {
        std::cout << "Parse error: " << request->errorMessage_ << std::endl;
    }

    if (path == "/login")
    {
        std::cout << "Processing /login for user: " << request->login_ << std::endl;

        auto loginResult = authService_->login(request->login_, request->password_);

        std::cout << "Login result success: " << loginResult.success << std::endl;

        if (loginResult.success)
        {
            request->setSuccess(true);
            request->jwtToken_ = loginResult.accessToken;
            request->refresh_token_ = loginResult.refreshToken;

            nlohmann::json responseJson;
            responseJson["success"] = true;
            responseJson["accessToken"] = loginResult.accessToken;
            responseJson["refreshToken"] = loginResult.refreshToken;
            responseJson["token_type"] = "Bearer";
            responseJson["expires_in"] = 900;
            request->responseBody_ = responseJson.dump();

            std::cout << "Login success, response prepared" << std::endl;
        }
        else
        {
            request->setSuccess(false);
            request->setErrorMessage(loginResult.errorMessage);

            nlohmann::json errorJson;
            errorJson["success"] = false;
            errorJson["error"] = loginResult.errorMessage.empty() ? "Login failed" : loginResult.errorMessage;
            request->responseBody_ = errorJson.dump();

            std::cout << "Login failed: " << loginResult.errorMessage << std::endl;
        }
    }
    else if (path == "/register")
    {
        std::cout << "Processing /register for user: " << request->login_ << std::endl;

        bool registred = authService_->registerUser(request->login_, request->email_, request->password_);

        if (registred)
        {
            request->setSuccess(true);
            nlohmann::json responseJson;
            responseJson["success"] = true;
            responseJson["message"] = "User registered successfully!";
            request->responseBody_ = responseJson.dump();
            std::cout << "Registration success" << std::endl;
        }
        else
        {
            request->setSuccess(false);
            request->setErrorMessage("Registration failed");
            nlohmann::json errorJson;
            errorJson["success"] = false;
            errorJson["error"] = "Registration failed";
            request->responseBody_ = errorJson.dump();
            std::cout << "Registration failed" << std::endl;
        }
    }
    else if (path == "/refreshToken")
    {
        auto refresh = authService_->validateToken(request->jwtToken_, request->refresh_token_);
        if (refresh.isValid)
        {
            request->setSuccess(true);

            request->jwtToken_ = refresh.accessToken;
            request->refresh_token_ = refresh.refreshToken;

            nlohmann::json responseJson;
            responseJson["success"] = true;
            responseJson["accessToken"] = request->jwtToken_;
            responseJson["refreshToken"] = request->refresh_token_;
            request->responseBody_ = responseJson.dump();
            std::cout << "Token refresh success" << std::endl;

        }
        else
        {
            request->setSuccess(false);
            request->setErrorMessage("Token refresh failed");
            nlohmann::json errorJson;
            errorJson["success"] = false;
            errorJson["error"] = "Token refresh failed";
            request->responseBody_ = errorJson.dump();
            std::cout << "Token refresh failed" << std::endl;
        }
    }
    else if (path == "/thread/create")
    {
        std::cout << "Processing /thread/create" << std::endl;

        // Create a new thread
        bool created = threadController_->createThread(request->title_, request->createdId_, request->uuid_);

        if (created)
        {
            request->setSuccess(true);
            nlohmann::json responseJson;
            responseJson["success"] = true;
            responseJson["message"] = "Thread created successfully!";
            request->responseBody_ = responseJson.dump();
            std::cout << "Thread creation success" << std::endl;
        }
        else
        {
            request->setSuccess(false);
            request->setErrorMessage("Thread creation failed");
            nlohmann::json errorJson;
            errorJson["success"] = false;
            errorJson["error"] = "Thread creation failed";
            request->responseBody_ = errorJson.dump();
            std::cout << "Thread creation failed" << std::endl;
        }
    }
    else if (path == "/thread/delete")
    {
        std::cout << "Processing /thread/delete" << std::endl;

        bool deleted = threadController_->deleteThread(request->threadId_);

        if (deleted)
        {
            request->setSuccess(true);
            nlohmann::json responseJson;
            responseJson["success"] = true;
            responseJson["message"] = "Thread deleted successfully!";
            request->responseBody_ = responseJson.dump();
            std::cout << "Thread deletion success" << std::endl;
        }
        else
        {
            request->setSuccess(false);
            request->setErrorMessage("Thread deletion failed");
            nlohmann::json errorJson;
            errorJson["success"] = false;
            errorJson["error"] = "Thread deletion failed";
            request->responseBody_ = errorJson.dump();
            std::cout << "Thread deletion failed" << std::endl;
        }
    }
    else if (path == "/thread/rename")
    {
		std::cout << "Processing /thread/rename" << std::endl;

		bool renamed = threadController_->updateTitleThread(request->threadId_,request->newTitle_);
        if (renamed)
        {
            request->setSuccess(true);
            nlohmann::json responseJson;
            responseJson["success"] = true;
            responseJson["message"] = "Thread renamed successfully!";
            request->responseBody_ = responseJson.dump();
            std::cout << "Thread renaming success" << std::endl;
        }
        else
        {
            request->setSuccess(false);
            request->setErrorMessage("Thread renaming failed");
            nlohmann::json errorJson;
            errorJson["success"] = false;
            errorJson["error"] = "Thread renaming failed";
            request->responseBody_ = errorJson.dump();
            std::cout << "Thread renaming failed" << std::endl;
        }
    }
    else
    {
        std::cout << "Unknown path: " << path << std::endl;
        request->setSuccess(false);
        request->setErrorMessage("Unknown path: " + path);

        nlohmann::json errorJson;
        errorJson["success"] = false;
        errorJson["error"] = "Unknown path: " + path;
        request->responseBody_ = errorJson.dump();
    }

    return request;
}