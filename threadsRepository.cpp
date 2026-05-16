#include "threadsRepository.h"

ThreadRepository::ThreadRepository(std::shared_ptr<Connections> conn)
{
	connection_ = conn;
}

bool ThreadRepository::createThread(const std::string& title, const int& creatorId, const std::string& uuid)
{
	auto conn = connection_->getConnection();
	try {
		pqxx::work txn(*conn);

		txn.exec(
			"INSERT INTO threads (id, title, created_by) VALUES ($1, $2, $3)",
			pqxx::params{uuid, title, creatorId}
		);

		txn.commit();
		connection_->returnConnection(conn);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in createThread: " << e.what() << std::endl;
		connection_->returnConnection(conn);
		return false;
	}
}

bool ThreadRepository::deleteThread(const std::string& threadId)
{
	auto conn = connection_->getConnection();
	try {
		pqxx::work txn(*conn);

		// Сначала проверяем, существует ли ветка с таким id
		pqxx::result checkResult = txn.exec(
			"SELECT id FROM threads WHERE id = $1",
			pqxx::params{ threadId }
		);

		// Если ветка не найдена — возвращаем false
		if (checkResult.empty())
		{
			std::cerr << "Thread not found with id: " << threadId << std::endl;
			return false;
		}

		// Ветка найдена, удаляем её
		txn.exec(
			"DELETE FROM threads WHERE id = $1",
			pqxx::params{ threadId }
		);

		txn.commit();
		connection_->returnConnection(conn);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in deleteThread: " << e.what() << std::endl;
		connection_->returnConnection(conn);
		return false;
	}
}

bool ThreadRepository::updateThread(const std::string& threadId, const std::string& newTitle)
{
	auto conn = connection_->getConnection();
	try {
		pqxx::work txn(*conn);

		// Сначала проверяем, существует ли ветка
		pqxx::result checkResult = txn.exec(
			"SELECT id FROM threads WHERE id = $1",
			pqxx::params{ threadId }
		);

		// Если ветка не найдена — возвращаем false
		if (checkResult.empty())
		{
			std::cerr << "Thread not found with id: " << threadId << std::endl;
			return false;
		}

		// Ветка найдена, обновляем её
		txn.exec(
			"UPDATE threads SET title = $1 WHERE id = $2",
			pqxx::params{ newTitle, threadId }
		);

		txn.commit();
		connection_->returnConnection(conn);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in updateThread: " << e.what() << std::endl;
		connection_->returnConnection(conn);
		return false;
	}
}

bool ThreadRepository::userSearch(const int& creatorId)
{
	auto conn = connection_->getConnection();
	try
	{
		pqxx::work txn(*conn);
		pqxx::result result = txn.exec(
			"SELECT id FROM users WHERE id = $1",
			pqxx::params{creatorId}
		);
		txn.commit();
		connection_->returnConnection(conn);
		return !result.empty();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in userSearch: " << e.what() << std::endl;
		connection_->returnConnection(conn);
		return false;
	}
}

nlohmann::json ThreadRepository::getTabByUserID(int& userId)
{
	auto conn = connection_->getConnection();
	try
	{
		pqxx::work txn(*conn);
		pqxx::result row = txn.exec(
			"SELECT id, title, created_at FROM threads WHERE created_by = $1 ORDER BY created_at DESC",
			pqxx::params{userId}
		);
		txn.commit();

		nlohmann::json result = nlohmann::json::array();

		for (const auto& rows : row)
		{
			nlohmann::json tab;
			tab["id"] = rows["id"].c_str();
			tab["title"] = rows["title"].c_str();
			tab["created_at"] = rows["created_at"].c_str();
			result.push_back(tab);
		}
		connection_->returnConnection(conn);
		return result;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in threads serch: " << e.what() << std::endl;
		connection_->returnConnection(conn);
		return nlohmann::json::array();
	}
}
