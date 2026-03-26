#ifndef DataBaseUsers_H
#define DataBaseUsers_H
#include <pqxx/pqxx>

class DataBaseUsers
{
	private:
		pqxx::connection& conn;
		 
	public:
		DataBaseUsers(pqxx::connection& c) : conn(c) {}

		void insert_users(pqxx::connection& conn, const std::string& login, const std::string& password);
};




#endif // Insert_users.H
