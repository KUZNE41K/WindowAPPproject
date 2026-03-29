#include <iostream>
#include <fstream>
#include <string>
#include "generate_salt.h"
#include "Hash.h"
#include "DataBaseUsers.h"
#include <pqxx/pqxx>
#include <windows.h>
#include "Connection.h"
#include "UserValidator.h"



int main(int argc, char* argv[])
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

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
	return 0;


}