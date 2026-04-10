#include <iostream>
#include <fstream>
#include <string>

#include "AuthService.h"
#include "Connection.h"


extern "C" __declspec(dllimport) int __stdcall SetConsoleCP(unsigned int wCodePageID); // не трогать

int main()
{
	std::string user = "djpj";
	std::string password = "password123";
	
	 // не трогать
	SetConsoleCP(65001); // не трогать

	AuthService authService(std::make_shared<UserRepository>(std::make_shared<Connections>()));
	authService.registerUser(user,password);
	authService.login(user,password);



	return 0;


}