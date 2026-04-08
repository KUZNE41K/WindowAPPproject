#include <iostream>
#include <fstream>
#include <string>
#include "UserRepository.h"
#include "DataBaseUsers.h"
#include <pqxx/pqxx>
#include "Connection.h"
#include "Hash.h"
#include "CreateSessionHandler.h"
#include "CreateUserHandler.h"
#include "User.h"
#include "Request.h"
#include "Handler.h"
#include "ValidationHandler.h"
#include "UserFetchHandler.h"
#include "UserNotExistsHandler.h"




int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	// конект к БД
	auto db = std::make_shared<Connections>();
	db->connectDataBase();

	auto userRepo = std::make_shared<UserRepository>(db);

	// рега

	ValidationHandler registrationHandler;
	UserFetchHandler regFetch(userRepo);
	UserNotExistsHandler regNotExists;
	CreateUserHandler createUser(userRepo);

	registrationHandler.setNext(std::make_shared<UserFetchHandler>(regFetch));
	regFetch.setNext(std::make_shared<UserNotExistsHandler>(regNotExists));
	regNotExists.setNext(std::make_shared<CreateUserHandler>(createUser));

	Request reqistrationRequest("vfvfvfvfv", "dcdcd", "password123");
	registrationHandler.handle(reqistrationRequest);

	if(reqistrationRequest.success_)
	{
		std::cout << "User registered successfully!" << std::endl;
	}
	else
	{
		std::cout << "Registration failed: " << reqistrationRequest.errorMessage_ << std::endl;
	}
	


	












	/*
	std::string password;

	// Ввод пароля (регистрация)
	std::cout << "Введите пароль для регистрации: ";
	std::cin >> password;

	// Хэширование
	std::string hash = Hash::hash(password);

	std::cout << "\nСохранённый хэш:\n" << hash << std::endl;

	//  Ввод пароля (вход)
	std::string input;
	std::cout << "\nВведите пароль для входа: ";
	std::cin >> input;

	//  Проверка
	if (Hash::verifyPassword(input, hash)) {
		std::cout << "Доступ разрешён " << std::endl;
	}
	else {
		std::cout << "Неверный пароль " << std::endl;
	}
*/
	/*
	Connections db;

	if (!db.connectDataBase())
	{
		std::cerr << "Failed to connect to the database." << std::endl;
		return 1;
	}

	try
	{
		auto& conn = db.getConnection();
		pqxx::work transaction(conn);

		auto result = transaction.exec("SELECT version()");
		transaction.commit();

		if (!result.empty())
		{
			std::cout << "PostgreSQL version: " << result[0][0].as<std::string>() << std::endl;
		}
		/*
		try
		{
			pqxx::work transactionsCreateTable(conn);

			transactionsCreateTable.exec("CREATE TABLE IF NOT EXISTS users ("
				"iser_id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,"
				"login VARCHAR(255) NOT NULL UNIQUE,"
				"email VARCHAR(255) NOT NULL UNIQUE,"
				"password VARCHAR(255) NOT NULL,"
				"salt VARCHAR(255) NOT NULL"
				")");

			transactionsCreateTable.commit();
			std::cout << "Table 'users' created successfully." << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error creating table: " << e.what() << std::endl;
		}
		try {
			DataBaseUsers dbUsers(conn);
			std::string login = "test_user";
			std::string email = "kuzmebkovniki1ta@gmail.com";
			std::string password = "1212";
			std::string salt = generate_salt();

			dbUsers.insert_users(conn, login, email, password, salt);

		}
		catch (const std::exception& e)
		{
			std::cerr << "Error inserting user: " << e.what() << std::endl;
		}
		*/
	/*
		std::string inputLogin = "test_user";

		User user = DataBaseUsers::gerUserByLogin(conn, inputLogin);

		if (UserValidator::identification(user,inputLogin))
		{
			std::cout << "Identification successful for user: " << user.login_ << std::endl;
		}
		else
		{
			std::cout << "Identification failed for user: " << user.login_ << std::endl;
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << "Database query error: " << e.what() << std::endl;
	}

	*/
	return 0;


}