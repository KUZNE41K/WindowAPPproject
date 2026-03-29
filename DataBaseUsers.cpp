#include "DataBaseUsers.h"

void DataBaseUsers::insert_users(pqxx::connection& conn, const std::string& login, const std::string& email, const std::string& password,const std::string& salt)
{
	try
	{
		pqxx::work txn(conn);
		pqxx::result res = txn.exec("INSERT INTO users(login,email,password,salt) VALUES($1,$2,$3,$4)",
			pqxx::params{ login, email,password,salt });
		std::cerr << "User inserted successfully." << std::endl;
		txn.commit();
	}
	catch (const std::exception&)
	{
		std::cout << "Error inserting user." << std::endl;
	}
}
