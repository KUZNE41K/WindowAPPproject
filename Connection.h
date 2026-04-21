#pragma once

#include <pqxx/pqxx>
#include <iostream>
#include <memory>



class Connections
{
private:
	std::shared_ptr<pqxx::connection> connection_;
	std::string connection_string_;
	void loadConnectionString();
public:
	Connections();//конструктор
	~Connections();//деструктор

	bool connectDataBase(); //метод для подключения к БД
	void disconnectDataBase(); //метод для отключения от БД
	bool isConnected() const; //метод для проверки подключения к БД

	pqxx::connection& getConnection();



};