#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>

#include "HttpServer.h"
#include "Router.h"
#include "AuthService.h"

namespace net = boost::asio;
using tcp = net::ip::tcp;

extern "C" __declspec(dllimport) int __stdcall SetConsoleCP(unsigned int wCodePageID); // не трогать

int main()
{
	try
	{
		net::io_context io_context;

		auto db = std::make_shared<Connections>();
		db->connectDataBase();

		auto userRepo = std::make_shared<UserRepository>(db);

		auto authService = std::make_shared<AuthService>(userRepo);

		auto router = std::make_shared<Router>(authService);

		tcp::endpoint endpoint(tcp::v4(), 8080);

		HttpServer server(io_context, endpoint, router);
		
		server.run();

		std::cout << "Server is running on port 8080..." << std::endl;

		io_context.run();

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}




	return 0;


}