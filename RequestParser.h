#ifndef REQUESTPARSER_H
#define REQUESTPARSER_H

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "Request.h"

class RequestParser {

public:
	static std::shared_ptr<Request> parseRequest(const std::string& body);
};






#endif // !REQUESTPARSER_H

