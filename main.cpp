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
#include "UserExistsHandler.h"
#include "PasswordCheckHandler.h"


extern "C" __declspec(dllimport) int __stdcall SetConsoleCP(unsigned int wCodePageID); // не трогать

int main()
{
	
	SetConsoleOutputCP(65001); // не трогать
	SetConsoleCP(65001); // не трогать
	
	// конект к БД
	
	auto db = std::make_shared<Connections>();
	db->connectDataBase();
	
	auto userRepo = std::make_shared<UserRepository>(db);

	// рега
	/*
	auto registrationValidation = std::make_shared<ValidationHandler>();
	auto regFetch = std::make_shared<UserFetchHandler>(userRepo);
	auto regNotExists = std::make_shared<UserNotExistsHandler>();
	auto createUser = std::make_shared<CreateUserHandler>(userRepo);


	registrationValidation->setNext(regFetch);
	regFetch->setNext(regNotExists);
	regNotExists->setNext(createUser);

	Request reqistrationRequest("vfvfvfv11f1v12", "dcdc1d1212@gmail.com", "password123");
	registrationValidation->handle(reqistrationRequest);

	if(reqistrationRequest.success_)
	{
		std::cout << "User registered successfully!" << std::endl;
	}
	else
	{
		std::cout << "Registration failed: " << reqistrationRequest.errorMessage_ << std::endl;
	}
*/


	//вход
	auto loginValidation = std::make_shared<ValidationHandler>();
	auto loginFetch = std::make_shared<UserFetchHandler>(userRepo);
	auto loginExists = std::make_shared<UserExistsHandler>();
	auto passwordCheck = std::make_shared<PasswordCheckHandler>();
	auto createSession = std::make_shared<CreateSessionHandler>(userRepo); // генерирует JWT и сохраняет в БД

	loginValidation->setNext(loginFetch);
	loginFetch->setNext(loginExists);
	loginExists->setNext(passwordCheck);
	passwordCheck->setNext(createSession);

	// создаём Request для входа
	Request loginRequest("vfvfvfv11f1v12", "password123"); // логин и пароль

	// запускаем цепочку
	loginValidation->handle(loginRequest);

	if (loginRequest.success_)
	{
		std::cout << "Login success! JWT: " << loginRequest.jwtToken_ << std::endl;
	}
	else
	{
		std::cout << "Login failed: " << loginRequest.errorMessage_ << std::endl;
	}


	// закончить тесты входа и регистрации
	


	


	












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