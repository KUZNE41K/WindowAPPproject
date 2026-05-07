#include "messagesRepository.h"

MessagesRepository::MessagesRepository(std::shared_ptr<Connections> conn)
{
	connection_ = conn;
}

bool MessagesRepository::createMessage(std::string threadId, int& userId, std::string& content)
{
	try 
	{
		pqxx::work txn(connection_->getConnection());

		txn.exec(
			"INSERT INTO messages_thread (thread_id, user_id, content) VALUES ($1, $2, $3)",
			pqxx::params{ threadId, userId,content }
		);

		txn.commit();
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in createMessage: " << e.what() << std::endl;
		return false;
	}
}

bool MessagesRepository::updateMessage(std::string threadId, int& messageId, const std::string& newContent)
{
	try
	{
		pqxx::work txn(connection_->getConnection());

		txn.exec(
			"UPDATE messages_thread SET content = $1 WHERE id = $2 AND thread_id = $3",
			pqxx::params{ newContent,  messageId ,threadId }
		);

		txn.commit();
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in updateMessage: " << e.what() << std::endl;
		return false;
	}
}

bool MessagesRepository::deleteMessage(std::string threadId, int& messageId)
{
	try
	{
		pqxx::work txn(connection_->getConnection());

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


	}
	catch(const std::exception& e)
	{
		std::cerr << "Error in deleteMessage: " << e.what() << std::endl;
		return false;
	}
}
