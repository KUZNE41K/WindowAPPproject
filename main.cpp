#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>

#include "HttpServer.h"
#include "Router.h"
#include "AuthService.h"

#include "SharedState.h"
#include "Listener.h"

namespace net = boost::asio;
using tcp = net::ip::tcp;

extern "C" __declspec(dllimport) int __stdcall SetConsoleCP(unsigned int wCodePageID); // не трогать

int main()
{
	try
	{
		net::io_context io_context;
		// база данных
		auto db = std::make_shared<Connections>();
		db->connectDataBase();

		auto userRepo = std::make_shared<UserRepository>(db);

		auto authService = std::make_shared<AuthService>(userRepo);
		// HTTP server
		auto router = std::make_shared<Router>(authService);

		tcp::endpoint endpoint(tcp::v4(), 8080);

		HttpServer server(io_context, endpoint, router);
		
		server.run();

		std::cout << "Server is running on port 8080..." << std::endl;

		// WebSocket server

		//auto sharedState = std::make_shared<SharedState>("./public");
		//auto wsListener = std::make_shared<Listener>(io_context,
		//	"0.0.0.0",
		//	8081,
		//	sharedState,
		//	"./public"
		//);
		//wsListener->run();
		//std::cout << "WebSocket server is running on port 8081..." << std::endl;


		//// запуск event loop
		io_context.run();

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}




	return 0;


}