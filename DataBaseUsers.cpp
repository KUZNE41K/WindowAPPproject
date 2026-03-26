#include "DataBaseUsers.h"
#include <pqxx/pqxx>

void DataBaseUsers::insert_users(pqxx::connection& conn, const std::string& login, const std::string& password)
{
	pqxx::work txn(conn);
	pqxx::result res = txn.exec("INSERT INTO users(login,password) VALUES($1,$2)",
		pqxx::params{ login, password });
	txn.commit();
}