#ifndef ROUTER_H
#define ROUTER_H
#include <memory>
#include <string>
#include "Request.h"
#include "AuthService.h"

class Router {
public:
	Router(std::shared_ptr<AuthService> authService);

	std::shared_ptr<Request> route(const std::string &path,
		std::string& body);



private:
	std::shared_ptr<AuthService> authService_;
};





#endif // !ROUTER_H
