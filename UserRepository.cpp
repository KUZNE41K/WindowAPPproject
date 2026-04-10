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
				row["password_hash"].as<std::string>()
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
		// Подготовка запроса (один раз, при старте программы)
		pqxx::work txn(connection_->getConnection());

		txn.exec(
			"INSERT INTO users (login, email, password_hash) VALUES ($1, $2, $3)",
			pqxx::params{ user->getLogin(), user->getEmail(), user->getPasswordHash() }
		);

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

bool UserRepository::saveSession(const std::string& userId, const std::string& token, int lifetimeSeconds)
{
	try {
		pqxx::work txn(connection_->getConnection());

		auto now = std::chrono::system_clock::now();
		auto expires_time_t = std::chrono::system_clock::to_time_t(now + std::chrono::seconds(lifetimeSeconds));
#if defined(_MSC_VER)
		std::tm expires_tm;
		gmtime_s(&expires_tm, &expires_time_t);
#else
		std::tm expires_tm = *std::gmtime(&expires_time_t);
#endif
		char buffer[20];
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &expires_tm);
		std::string expires_at_str(buffer);

		txn.exec(
			"INSERT INTO sessions (user_id, token, issued_at, expires_at, revoked) "
			"VALUES ($1, $2, NOW(), $3, FALSE);",
			pqxx::params{userId, token, expires_at_str}
		);
		txn.commit();
		return true;

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in saveSession: " << e.what() << std::endl;
		return false;
	}
}
