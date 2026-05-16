#pragma once

#include <pqxx/pqxx>
#include <iostream>
#include <memory>
#include "ConnectionPool.h"



class Connections
{
private:
	std::shared_ptr<ConnectionPool> pool_;
	std::string connection_string_;
	void loadConnectionString();
public:
	Connections();//конструктор
	~Connections();//деструктор

	bool connectDataBase(); //метод для подключения к БД
	void disconnectDataBase(); //метод для отключения от БД
	bool isConnected() const; //метод для проверки подключения к БД

	void returnConnection(std::shared_ptr<pqxx::connection> conn);
	pqxx::connection getConnectionOld();

	std::shared_ptr<pqxx::connection> getConnection();



};