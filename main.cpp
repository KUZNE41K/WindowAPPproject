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



int main()
{
	try
	{
		net::io_context io_context;
		// база данных
		auto db = std::make_shared<Connections>();
		db->connectDataBase();
		// рега и вход
		auto userRepo = std::make_shared<UserRepository>(db);
		auto authService = std::make_shared<AuthService>(userRepo);
		// ветки 
		auto threadsService = std::make_shared<ThreadRepository>(db);
		auto threadController = std::make_shared<ThreadController>(threadsService);
		//сообщения в ветках
		auto messageThreadsService = std::make_shared<MessagesRepository>(db);
		auto messageThreadsController = std::make_shared<MessageThreadController>(messageThreadsService);
		// HTTP server
		auto router = std::make_shared<Router>(authService,threadController,messageThreadsController);

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
