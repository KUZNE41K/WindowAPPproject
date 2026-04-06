#include "UserRepository.h"

std::string UserRepository::buildSelectQuery(const std::string& table, const std::string& condition)
{
	return "SELECT * FROM "+ table + " WHERE " + condition + " ; ";
}

std::shared_ptr<User> UserRepository::findUserByField(const std::string& field, const std::string& value)
{
	if (!connection_ || !connection_->isConnected())
	{
		std::cerr << "Database connection is not established." << std::endl;
		return nullptr;
	}

	try
	{
		std::string query = buildSelectQuery("users", field + " = '" + value + "'");
		pqxx::work txn(connection_->getConnection());
		pqxx::result result = txn.exec(query);

		if (result.size() == 1)
		{
			const auto& row = result[0];
			auto user = std::make_shared<User>(
				row["id"].as<int>(),
				row["login"].as<std::string>(),
				row["email"].as<std::string>(),
				row["password"].as<std::string>()
			);
			return user;
		}
		return nullptr;


	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in findUserByField: " << e.what() << std::endl;
		return nullptr;
	}
}

UserRepository::UserRepository(std::shared_ptr<Connections> conn)
{
	connection_ = conn;
}

std::shared_ptr<User> UserRepository::findUserByLogin(const std::string& login)
{
	return findUserByField("login", login);
}

std::shared_ptr<User> UserRepository::findUserByEmail(const std::string& email)
{
	return findUserByField("email", email);
}

bool UserRepository::saveUser(std::shared_ptr<User>user)
{
	try {
		pqxx::work txn(connection_->getConnection());
		txn.exec("INSERT INTO users (login, email, password) VALUES ($1, $2, $3);"),
			user ->getLogin(),user ->getEmail(), user->getPasswordHash(); // Здесь нужно будет передать реальные данные пользователя
		txn.commit();
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in saveUser: " << e.what() << std::endl;
		return false;
	}
}

bool UserRepository::updateUser()// потом
{
	return false;
}
