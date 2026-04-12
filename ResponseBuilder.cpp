#include "ResponseBuilder.h"

std::string ResponseBuilder::buildResponse(const Request& request)
{
	nlohmann::json json;

	json["success"] = request.success_;

	if (!request.errorMessage_.empty())
	{
		json["error"] = request.errorMessage_;
	}

	if (request.user_ != nullptr)
	{
		json["user"] = {
			{"id", request.user_->getId()},
			{"login", request.user_->getLogin()},
			{"email", request.user_->getEmail()}
		};
	}

	if(!request.jwtToken_.empty())
	{
		json["jwtToken"] = request.jwtToken_;
	}

	return json.dump();
}
