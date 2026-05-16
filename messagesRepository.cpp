#include "messagesRepository.h"

MessagesRepository::MessagesRepository(std::shared_ptr<Connections> conn)
{
	connection_ = conn;
}

bool MessagesRepository::createMessage(std::string threadId, int& userId, std::string& content)
{
	auto conn = connection_->getConnection();
	
	try 
	{
		pqxx::work txn(*conn);

		txn.exec(
			"INSERT INTO messages_thread (thread_id, user_id, content) VALUES ($1, $2, $3)",
			pqxx::params{ threadId, userId,content }
		);

		txn.commit();
		connection_->returnConnection(conn);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in createMessage: " << e.what() << std::endl;
		connection_->returnConnection(conn);
		return false;
	}
}

bool MessagesRepository::updateMessage(std::string threadId, int& messageId, const std::string& newContent)
{
	auto conn = connection_->getConnection();
	try
	{
		pqxx::work txn(*conn);

		txn.exec(
			"UPDATE messages_thread SET content = $1 WHERE id = $2 AND thread_id = $3",
			pqxx::params{ newContent,  messageId ,threadId }
		);

		txn.commit();
		connection_->returnConnection(conn);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in updateMessage: " << e.what() << std::endl;
		connection_->returnConnection(conn);
		return false;
	}
}

bool MessagesRepository::deleteMessage(std::string threadId, int& messageId)
{
	auto conn = connection_->getConnection();
	try
	{
		pqxx::work txn(*conn);

		// —начала провер€ем, существует ли сообщение с таким id и в такой ведке
		pqxx::result checkResult = txn.exec(
			"SELECT id FROM messages_thread WHERE id = $1 AND thread_id = $2 ",
			pqxx::params{ messageId, threadId }
		);

		// ≈сли ветка не найдена Ч возвращаем false
		if (checkResult.empty())
		{
			std::cerr << "Thread not found with id: " << threadId << std::endl;
			return false;
		}
		
		txn.exec(
			"DELETE FROM messages_thread WHERE id = $1 and thread_id = $2",
			pqxx::params{ messageId, threadId }
		);
		txn.commit();
		connection_->returnConnection(conn);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error in deleteMessage: " << e.what() << std::endl;
		connection_->returnConnection(conn);
		return false;
	}
}

nlohmann::json MessagesRepository::getbMessageByThreadId(const std::string& threadId)
{
	auto conn = connection_->getConnection();
	try
	{
		pqxx::work txn(*conn);
		pqxx::result row = txn.exec(
			"SELECT id, content, created_at FROM messages_thread WHERE thread_id = $1 ORDER BY created_at DESC",
			pqxx::params{ threadId }
		);
		txn.commit();

		nlohmann::json result = nlohmann::json::array();

		for (const auto& rows : row)
		{
			nlohmann::json tab;
			tab["id"] = rows["id"].c_str();
			tab["content"] = rows["content"].c_str();
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
