#include "RequestParser.h"

std::shared_ptr<Request> RequestParser::parseRequest(const std::string& body)
{
	auto request = std::make_shared<Request>();

	try {
		auto json = nlohmann::json::parse(body);
		if (json.contains("login") && json.contains("password")) {
			request->login_ = json["login"].get<std::string>();
			request->password_ = json["password"].get<std::string>();
		}
		else if (json.contains("email") && json.contains("password")) {
			request->email_ = json["email"].get<std::string>();
			request->password_ = json["password"].get<std::string>();
		}
		else {
			request->setErrorMessage("Invalid request format: missing required fields.");
		}
	}
	catch(const std::exception&e)
	{
		request->setSuccess(false);
		request->setErrorMessage("Failed to parse JSON request.");
	}
	return request;
}
