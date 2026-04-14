#include "RequestParser.h"

std::shared_ptr<Request> RequestParser::parseRequest(const std::string& body)
{
	auto request = std::make_shared<Request>();

	try {
		auto json = nlohmann::json::parse(body);
		std::string type = json.value("type", " ");

        if (type == "register")
        {
            request->login_ = json.value("login", "");
            request->email_ = json.value("email", "");
            request->password_ = json.value("password", "");
        }
        else if (type == "login")
        {
            request->login_ = json.value("login", "");
            request->password_ = json.value("password", "");
        }
        else
        {
            request->setSuccess(false);
            request->setErrorMessage("Unknown request type");
        }
	}
	catch(const std::exception&e)
	{
		request->setSuccess(false);
		request->setErrorMessage("Failed to parse JSON request.");
	}
	return request;
}
