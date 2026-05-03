#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H
#include "Connection.h"
#include "Request.h"
#include "User.h"
#include <memory>
#include <string>
#include <chrono>

struct SessionInfo {
	int id;
	std::string userId;
	std::string tokenHash;
	std::string issuedAt;
	std::string expiresAt;
	bool used;
	std::string parentTokenHash;
	bool revoked;
	std::string createdAt;
};

class UserRepository
{
private:
	std::shared_ptr<Connections> connection_;
	std::string buildSelectQuery(const std::string& table,const std::string& condition);

	std::shared_ptr<User> findUserByField(const std::string& field, const std::string& value);

	


public:
	UserRepository(std::shared_ptr<Connections> conn);
	std::shared_ptr<User> findUserByLogin(const std::string& login);
	std::shared_ptr<User> findUserByEmail(const std::string& email);
	bool saveUser(std::shared_ptr<User>user);
	bool updateUser();// потом
	bool saveSession(const std::string& userId, const std::string& token, const std::string& parentTokenHash, int lifetimeSeconds);
	bool rotateRefreshToken(const std::string& userId,const std::string& oldRefreshToken, const std::string& newRefreshToken,int lifetimeSeconds);
	std::optional<SessionInfo> findSessionByToken(const std::string& token);
	std::optional<SessionInfo> findSessionByTokenHash(const std::string& tokenHash);


};





#endif // !USERREPOSITORY_H

