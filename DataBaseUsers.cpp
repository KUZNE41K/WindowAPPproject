#include "DataBaseUsers.h"

void DataBaseUsers::insert_users(pqxx::connection& conn, const std::string& login, const std::string& email, const std::string& password,const std::string& salt)
{
	try
	{
		pqxx::work txn(conn);
		pqxx::result res = txn.exec("INSERT INTO users(login,email,password,) VALUES($1,$2,$3)",
			pqxx::params{ login, email,password });
		std::cerr << "User inserted successfully." << std::endl;
		txn.commit();
	}
	catch (const std::exception&)
	{
		std::cout << "Error inserting user." << std::endl;
	}
}

User DataBaseUsers::gerUserByLogin(pqxx::connection& conn, const std::string& login)
{
	pqxx::work txn(conn);

	pqxx::result res = txn.exec("SELECT id, login, email, password FROM users WHERE login = $1",
		pqxx::params{ login });
	txn.commit();

	if (res.empty())
	{
		throw std::runtime_error("User not found");
	}
	return User(
		res[0][0].as<int>(), // id
		res[0][1].as<std::string>(), // login
		res[0][2].as<std::string>(), // email
		res[0][3].as<std::string>()  // passwordHash
	);
}
