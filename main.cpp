#include <iostream>
#include <fstream>
#include <string>
#include "generate_salt.h"
#include "Hash.h"
#include "DataBaseUsers.h"
#include <pqxx/pqxx>
#include <windows.h>



int main(int argc, char* argv[])
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	std::string salt = generate_salt();
	std::cout << salt;

	std::string password = "jfbvvjfn";
	std::cout << salt << "\n";
	std::cout << password << "\n";

	std::string Hash = hashPasswordWithSalt(password,salt);
	std::cout << Hash;

	try
	{
		pqxx::connection cx("dbname = postgres user = postgres password = kuzykuzn hostaddr = 127.0.0.1 port = 5432");

		if (cx.is_open()) {
			std::cout << "Opened database successfully: " << cx.dbname() << std::endl;
		}
		else {
			std::cout << "Can't open database" << std::endl;
			return 1;
		}

		/*
		std::string sql = "CREATE TABLE USERS ("
			"ID SERIAL PRIMARY KEY     NOT NULL," \
			"LOGIN           TEXT    NOT NULL," \
			"PASSWORD        TEXT    NOT NULL);";
		*/

		std::string login = "admin";
		std::string password = Hash;


		DataBaseUsers users(cx);
		users.insert_users(cx, login, password);

	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
}