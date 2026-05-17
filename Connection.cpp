#include "Connection.h"



Connections::Connections() : connection_(nullptr)
{
	loadConnectionString();
}

Connections::~Connections()
{
	disconnectDataBase();
}

void Connections::loadConnectionString()
{
	connection_string_ = "dbname=myapp_db user=myapp_user password=kuzykuzn host=postgres port=5432";
}

bool Connections::connectDataBase()
{
	try {

		connection_ = std::make_shared<pqxx::connection>(connection_string_);
		if (connection_->is_open()) {
			std::cout << "Opened database successfully: " << connection_->dbname() << std::endl;
			return true;
		}
		else {
			std::cout << "Can't open database" << std::endl;
			connection_.reset();
			return false;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Database connection error" << std::endl;
		connection_.reset();
		return false;
	}
	
}

void Connections::disconnectDataBase()
{
	if (connection_ && connection_->is_open())
	{
		connection_->close();
		std::cout << "Database connection closed." << std::endl;
	}
	connection_.reset();
}

bool Connections::isConnected() const
{
	return connection_ && connection_->is_open();
}

pqxx::connection& Connections::getConnection()
{
	if (!isConnected())
	{
		throw std::runtime_error("Database connection is not established.");
	}
	return *connection_;
}

