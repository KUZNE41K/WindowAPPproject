#ifndef CREATESESSIONHANDLER_H
#define CREATESESSIONHANDLER_H
#include "Handler.h"
#include "Request.h"
#include "UserRepository.h"
#include <string>
#include <jwt-cpp/jwt.h>
#include <chrono>

class CreateSessionHandler : public Handler
{
public:
	explicit CreateSessionHandler(const std::string& jwtSecret, std::shared_ptr<Connections> conn);
	void handle(Request& request) override;

private:
	std::string jwtSecret_;
	UserRepository repo_; 
	std::string generateJwtToken(const std::string &userId);
	bool saveSessionToDatabase(const std::string& userId, const std::string& token, int lifetimeSeconds);
};


#endif // !CREATESESSIONHANDLER_H
