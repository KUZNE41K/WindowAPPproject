#include "messagesRepository.h"

MessagesRepository::MessagesRepository(std::shared_ptr<Connections> conn)
{
	connection_ = conn;
}

bool MessagesRepository::createMessage(std::shared_ptr<Messages> message)
{
	try 
	{
		pqxx::work txn(connection_->getConnection());

		txn.exec(
			"INSERT INTO messages (content, user_id) VALUES ($1, $2)",
			pqxx::params{ message->getContent(), message->getAuthorId() }
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

bool MessagesRepository::updateMessage(int& messageId, const std::string& newContent)
{
	try
	{
		pqxx::work txn(connection_->getConnection());

		txn.exec(
			"UPDATE messages SET content = $1 WHERE id = $2",
			pqxx::params{ newContent, messageId }
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
