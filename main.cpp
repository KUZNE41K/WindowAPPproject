#include <iostream>
#include <fstream>
#include <string>
#include "RequestParser.h"


#include "AuthService.h"
#include "Connection.h"


extern "C" __declspec(dllimport) int __stdcall SetConsoleCP(unsigned int wCodePageID); // не трогать

int main()
{
	std::string jsonRequest = R"({"login": "user1", "password": "pass123"})";

	auto request = RequestParser::parseRequest(jsonRequest);
	std::cout << "Login " << request->login_ << std::endl;
	std::cout << "Password " << request->password_ << std::endl;

	return 0;


}