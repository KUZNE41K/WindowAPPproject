#include "threadsRepository.h"

ThreadRepository::ThreadRepository(std::shared_ptr<Connections> conn)
{
	connection_ = conn;
}

bool ThreadRepository::createThread(const std::string& title, const int& createdId, const std::string& uuid)
{
	try {
		pqxx::work txn(connection_->getConnection());

		txn.exec(
			"INSERT INTO users (title, creatorId, uuid) VALUES ($1, $2, $3)",
			pqxx::params{title, createdId, uuid}
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

bool ThreadRepository::deleteThread(int& threadId)
{
	try {
		pqxx::work txn(connection_->getConnection());

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

bool ThreadRepository::updateThread(int& threadId, const std::string& newTitle)
{
	try {
		pqxx::work txn(connection_->getConnection());

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

bool ThreadRepository::userSearch(int& creatorId)
{
	try
	{
		pqxx::work txn(connection_->getConnection());
		pqxx::result result = txn.exec(
			"SELECT id FROM users WHERE creatorId = $1",
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
