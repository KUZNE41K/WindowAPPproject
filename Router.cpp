#include "Router.h"
#include "RequestParser.h"
#include <iostream>  //  Добавить

Router::Router(std::shared_ptr<AuthService> authService) : authService_(authService)
{
}

std::shared_ptr<Request> Router::route(const std::string& path, std::string& body)
{
    //  Добавить отладочный вывод
    std::cout << "=== Router::route ===" << std::endl;
    std::cout << "Path: " << path << std::endl;
    std::cout << "Body: " << body << std::endl;

    auto request = RequestParser::parseRequest(body);

    // Проверить результат парсинга
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