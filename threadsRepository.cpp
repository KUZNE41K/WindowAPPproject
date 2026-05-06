#include "threadsRepository.h"

ThreadRepository::ThreadRepository(std::shared_ptr<Connections> conn)
{
	connection_ = conn;
}

bool ThreadRepository::createThread(const std::string& title, const int& creatorId, const std::string& uuid)
{
	try {
		pqxx::work txn(connection_->getConnection());

		txn.exec(
			"INSERT INTO threads (id, title, created_by) VALUES ($1, $2, $3)",
			pqxx::params{uuid, title, creatorId}
		);

		txn.commit();
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in createThread: " << e.what() << std::endl;
		return false;
	}
}

bool ThreadRepository::deleteThread(const std::string& threadId)
{
	try {
		pqxx::work txn(connection_->getConnection());

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
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in deleteThread: " << e.what() << std::endl;
		return false;
	}
}

bool ThreadRepository::updateThread(const std::string& threadId, const std::string& newTitle)
{
	try {
		pqxx::work txn(connection_->getConnection());

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
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in updateThread: " << e.what() << std::endl;
		return false;
	}
}

bool ThreadRepository::userSearch(const int& creatorId)
{
	try
	{
		pqxx::work txn(connection_->getConnection());
		pqxx::result result = txn.exec(
			"SELECT id FROM users WHERE id = $1",
			pqxx::params{creatorId}
		);
		txn.commit();
		return !result.empty();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in userSearch: " << e.what() << std::endl;
		return false;
	}
}
