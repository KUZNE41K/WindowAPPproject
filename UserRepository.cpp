#include "UserRepository.h"
#include "Hash.h"

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
		else
		{
			return nullptr;
		}
		


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

bool UserRepository::saveSession(const std::string& userId, const std::string& refreshToken, const std::string& parentTokenHash, int lifetimeSeconds)
{

	if (userId.empty() || refreshToken.empty() || lifetimeSeconds <= 0 )
	{
		std::cerr << "Invalid input parameters for saveSession." << std::endl;
		return false;
	}
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

		if (parentTokenHash.empty())
		{
			txn.exec(
				"INSERT INTO sessions1 (user_id, token_hash, issued_at, expires_at, revoked) "
				"VALUES ($1, $2, NOW(), $3, FALSE) ",
				pqxx::params{ userId, refreshToken, expires_at_str }
			);
			txn.commit();
			return true;
		}
		else
		{
			txn.exec(
				"INSERT INTO sessions1 (user_id, token_hash, parent_token_hash, issued_at, expires_at, revoked) "
				"VALUES ($1, $2, $3, NOW(), $4, FALSE) ",
				pqxx::params{ userId, refreshToken, parentTokenHash, expires_at_str }
			);
			txn.commit();
			return true;
		}
		

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in saveSession: " << e.what() << std::endl;
		return false;
	}
}

bool UserRepository::rotateRefreshToken(const std::string& userId, const std::string& oldRefreshToken, const std::string& newRefreshToken, int lifetimeSeconds)
{
	try 
	{
		pqxx::work txn(connection_->getConnection());

		txn.exec(
			"UPDATE sessions1 SET revoked = TRUE "
			"WHERE token_hash = $1 AND user_id = $2 ",
			pqxx::params{ Hash::hashToken(oldRefreshToken), userId }
		);
		txn.commit();
		return saveSession(userId, newRefreshToken, oldRefreshToken, lifetimeSeconds);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in rotateRefreshToken: " << e.what() << std::endl;
		return false;
	}


	
}

std::optional<SessionInfo> UserRepository::findSessionByToken(const std::string& token)
{
	try
	{
		std::string tokenHash = Hash::hashToken(token);
		std::cout << tokenHash << "=== token hash ===" << std::endl;
		return findSessionByTokenHash(tokenHash);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in findSessionByToken: " << e.what() << std::endl;
		return std::nullopt;
	}
	
}

std::optional<SessionInfo> UserRepository::findSessionByTokenHash(const std::string& tokenHash)
{
	try
	{
		pqxx::work txn(connection_->getConnection());
		pqxx::result result = txn.exec(
			"SELECT id, user_id, token_hash, issued_at, expires_at, "
			"used, parent_token_hash, revoked, created_at "
			"FROM sessions1 WHERE token_hash = $1 "
			"AND revoked = false "
			"AND expires_at > NOW() ",
			pqxx::params{ tokenHash }
		);
		

		if (result.empty())
		{
			return std::nullopt;
		}
		else
		{
			SessionInfo sessionInfo;

			sessionInfo.id = result[0][0].as<int>();
			sessionInfo.userId = result[0][1].as<std::string>();
			sessionInfo.tokenHash = result[0][2].as<std::string>();
			sessionInfo.issuedAt = result[0][3].as<std::string>();
			sessionInfo.expiresAt = result[0][4].as<std::string>();
			sessionInfo.used = result[0][5].as<bool>();
			if(!result[0][6].is_null())
			{
				sessionInfo.parentTokenHash = result[0][6].as<std::string>();
			}
			sessionInfo.revoked = result[0][7].as<bool>();
			sessionInfo.createdAt = result[0][8].as<std::string>();
			return sessionInfo;

		}
		txn.commit();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in findSessionByTokenHash: " << e.what() << std::endl;
		return std::nullopt;
	}
}
