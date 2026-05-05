#ifndef ROUTER_H
#define ROUTER_H
#include <memory>
#include <string>
#include "Request.h"
#include "AuthService.h"
#include "ThreadController.h"
#include <nlohmann/json.hpp>
class Router {
public:
	Router(std::shared_ptr<AuthService> authService, std::shared_ptr<ThreadController> threadController);

	std::shared_ptr<Request> route(const std::string &path,
		std::string& body);



private:
	std::shared_ptr<AuthService> authService_;
	std::shared_ptr<ThreadController> threadController_;
};





#endif // !ROUTER_H
