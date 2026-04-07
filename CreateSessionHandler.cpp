#include "CreateSessionHandler.h"

CreateSessionHandler::CreateSessionHandler(const std::string& jwtSecret, std::shared_ptr<Connections> conn)
    : jwtSecret_(jwtSecret), repo_(conn){}

void CreateSessionHandler::handle(Request& request)
{
	if (!request.user_->isValid())
	{
		request.setSuccess(false);
		request.setErrorMessage("User not found for session creation.");
		return;
	}
	int lifetimeSeconds = 3600; // 1 hour, можно сделать параметром метода или константой
	std::string token = generateJwtToken(std::to_string(request.user_->getId()));
	saveSessionToDatabase(std::to_string(request.user_->getId()), token,lifetimeSeconds);// lifetimeSeconds добавить как параметр метода или использовать константное значение
	request.sessionToken_ = token;
	request.setSuccess(true);
	if (next_)
	{
		next_->handle(request);
	}
}

std::string CreateSessionHandler::generateJwtToken(const std::string& userId)
{
	auto now = std::chrono::system_clock::now();
	auto expires = now + std::chrono::hours(24); // Token valid for 24 hours

	auto token = jwt::create()
		.set_issuer("MyApp")
		.set_subject(userId)
		.set_issued_at(now)
		.set_expires_at(expires)
		.sign(jwt::algorithm::hs256{ jwtSecret_ });
	return token;
}

bool CreateSessionHandler::saveSessionToDatabase(const std::string& userId, const std::string& token, int lifetimeSeconds)
{
	return repo_.saveSession(userId, token,lifetimeSeconds);
}
