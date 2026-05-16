#include "Connection.h"



Connections::Connections() : pool_(nullptr)
{
	loadConnectionString();
}

Connections::~Connections()
{
	disconnectDataBase();
}

void Connections::loadConnectionString()
{
	connection_string_ = "dbname = postgres user = postgres password = kuzykuzn hostaddr = 127.0.0.1 port = 5432";
}

bool Connections::connectDataBase()
{
	try 
	{
		int pool_size = 30;
		std::cout << "Initializing database connection pool with " << pool_size << " connections..." << std::endl;
		pool_ = std::make_shared<ConnectionPool>(connection_string_, pool_size);

		if (pool_ && !pool_->empty())
		{
			std::cout << "Database connection pool successfully created with " << pool_->size() << " connections" << std::endl;
			return true;
		}
		else
		{
			std::cerr << "Failed to create database connection pool" << std::endl;
			pool_.reset();
			return false;
		}
	}
	catch (const std::exception&e)
	{
		std::cerr << "Database connection pool error: " << e.what() << std::endl;
		pool_.reset();
		return false;
	}
	
}

void Connections::disconnectDataBase()
{
	if (pool_)
	{
		pool_->shutdown();
		pool_.reset();
	}
	std::cout << "DataBase disconect" << std::endl;
}

bool Connections::isConnected() const
{
	return pool_ != nullptr && !pool_->empty();
}

void Connections::returnConnection(std::shared_ptr<pqxx::connection> conn)
{
	if (pool_ && conn && conn->is_open())
	{
		pool_->put(conn);
	}
}

pqxx::connection Connections::getConnectionOld()
{
	throw std::runtime_error("getConnectionOld() is deprecated. Use getConnection() and returnConnection() instead.");
}

std::shared_ptr<pqxx::connection> Connections::getConnection()
{
	if (!isConnected())
	{
		throw std::runtime_error("Database connection is not established.");
	}
	return pool_->get();
}

