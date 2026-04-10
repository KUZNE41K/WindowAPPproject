#ifndef DataBaseUsers_H
#define DataBaseUsers_H
#include <pqxx/pqxx>
#include "User.h"
#include <iostream>

class DataBaseUsers
{
	private:
		pqxx::connection& conn;
		 
	public:
		DataBaseUsers(pqxx::connection& c) : conn(c) {}

		void insert_users(pqxx::connection& conn, const std::string& login, const std::string& email, const std::string& password_hash);
		static User gerUserByLogin(pqxx::connection& conn, const std::string& login);

};




#endif // Insert_users.H
