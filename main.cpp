#include <iostream>
#include <fstream>
#include <string>
#include <thread>
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
		auto const thread_count = std::thread::hardware_concurrency();
		std::cout << thread_count << std::endl;
		net::io_context io_context(thread_count);
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

		std::vector<std::thread> threads;
		threads.reserve(thread_count);

		for (size_t i = 0; i < thread_count; i++)
		{
			threads.emplace_back([&io_context] {
				io_context.run();
				});
		}
		
		std::cin.get();
		io_context.stop();

		// 4. ЖДЕМ ЗАВЕРШЕНИЯ ВСЕХ ПОТОКОВ
		for (auto& t : threads) {
			t.join();
		}

		

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

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}




	return 0;


}