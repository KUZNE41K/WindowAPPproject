#include "RequestParser.h"
#include <iostream>

std::shared_ptr<Request> RequestParser::parseRequest(const std::string& body)
{
    auto request = std::make_shared<Request>();
    request->setSuccess(false);
    
    std::cout << "=== RequestParser ===" << std::endl;
    std::cout << "Raw body: " << body << std::endl;

    try {
        auto json = nlohmann::json::parse(body);
        
        std::string type = json.value("type", "");
        
        std::cout << "Type: '" << type << "'" << std::endl;

        if (type == "register")
        {
            request->login_ = json.value("login", "");
            request->email_ = json.value("email", "");
            request->password_ = json.value("password", "");
            request->setSuccess(true);
            std::cout << "Register request for: " << request->login_ << std::endl;
        }
        else if (type == "login")
        {
            request->login_ = json.value("login", "");
            request->password_ = json.value("password", "");
            request->setSuccess(true);
            std::cout << "Login request for: " << request->login_ << std::endl;
        }
        else if(type == "refresh")
        {
			request->refresh_token_ = json.value("refreshToken", "");
			request->jwtToken_ = json.value("accessToken", "");
			request->setSuccess(true);
			return request;
        }
        else if (type == "createThreads")
        {
			std::string title_ = json.value("title", "");
            std::string uuid_ = json.value("uuid", "");
            int createdId_ = json.value("createdId", 0);
			
            std::string errorMessage;
            if (!ThreadsValidator::validateCreate(title_, uuid_, createdId_,errorMessage))
            {
                request->setErrorMessage(errorMessage);
				request->setSuccess(false);
                return request;
            }
            
            request->title_ = title_;
            request->uuid_ = uuid_;
            request->createdId_ = createdId_;
			request->setSuccess(true);
			return request;
			
        }
        else if (type == "deleteThreads")
        {
            std::string threadId_ = json.value("threadId", "");

            std::string errorMessage;
            if (!ThreadsValidator::validateDelete(threadId_, errorMessage))
            {
                request->setErrorMessage(errorMessage);
                request->setSuccess(false);
                return request;
            }
            request->threadId_ = threadId_;
            request->setSuccess(true);
            return request;
        }
        else if (type == "renameThreads")
        {
            std::string threadId_ = json.value("threadId", "");
            std::string newTitle_ = json.value("newTitle", "");

            std::string errorMessage;
            if (!ThreadsValidator::validateUpdateRename(threadId_, newTitle_, errorMessage))
            {
                request->setErrorMessage(errorMessage);
                request->setSuccess(false);
                return request;
            }
            request->threadId_ = threadId_;
            request->newTitle_ = newTitle_;
            request->setSuccess(true);
            return request;
        }
        else if (type == "createMessageThread")
        {
            std::string threadId_ = json.value("threadId", "");
            int messageId_ = json.value("messageId", 0);
            int userId_ = json.value("userId", 0);
            std::string contentThread_ = json.value("contentThread", "");

            std::string errorMessage;
            if (!MessagesThreadValidator::validateCreate(threadId_,userId_,contentThread_,errorMessage))
            {
                request->setErrorMessage(errorMessage);
                request->setSuccess(false);
                return request;
            }
            request->threadId_ = threadId_;
            request->messageId_ = messageId_;
            request->userId_ = userId_;
            request->contentThread_ = contentThread_;
            request->setSuccess(true);
            return request;
        }
        else if (type == "deleteMessageThread")
        {
            std::string threadId_ = json.value("threadId", "");
            int messageId_ = json.value("messageId", 0);

            std::string errorMessage;
            if (!MessagesThreadValidator::validateDelete(threadId_,messageId_,errorMessage))
            {
                request->setErrorMessage(errorMessage);
                request->setSuccess(false);
                return request;
            }
            request->threadId_ = threadId_;
            request->messageId_ = messageId_;
            request->setSuccess(true);
            return request;
        }
        else if (type == "updateMessageThread")
        {
            std::string threadId_ = json.value("threadId", "");
            int messageId_ = json.value("messageId", 0);
            std::string newContent = json.value("newContent", "");

            std::string errorMessage;
            if (!MessagesThreadValidator::validateUpdateContent(threadId_, messageId_, newContent,errorMessage))
            {
                request->setErrorMessage(errorMessage);
                request->setSuccess(false);
                return request;
            }
            request->threadId_ = threadId_;
            request->messageId_ = messageId_;
            request->newContentThread_ = newContent;
            request->setSuccess(true);
            return request;
        }
        else if (type == "getThreads")
        {
            int userId_ = json.value("userId",0);
            request -> createdId_ = userId_;
            request->setSuccess(true);
        }
        else
        {
            std::cout << "Unknown type: '" << type << "'" << std::endl;
            request->setSuccess(false);
            request->setErrorMessage("Unknown request type: " + type);
        }
    }
    catch(const std::exception& e)
    {
        std::cout << "Parse exception: " << e.what() << std::endl;
        request->setSuccess(false);
        request->setErrorMessage("Failed to parse JSON request.");
    }
    
    std::cout << "Parser success: " << request->success_ << std::endl;
    return request;
}