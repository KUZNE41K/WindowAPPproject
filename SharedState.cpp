#include "SharedState.h"
#include "WebSocketSession.h"
#include <algorithm>
#include <string>
SharedState::SharedState(std::string docRoot): docRoot_(std::move(docRoot))
{
}

void SharedState::join(const std::string& sessionId, std::shared_ptr<WebSocketSession> sessions)
{
	std::lock_guard<std::mutex> lock(mutex_);
	cleanupDeadSession();

	SessionData data;
	data.session = std::weak_ptr<WebSocketSession>(sessions);
	sessions_[sessionId] = std::move(data);

}

void SharedState::leave(const std::string& sessionId)
{
	std::lock_guard<std::mutex> lock(mutex_);

	auto it = sessions_.find(sessionId);
	if (it != sessions_.end())
	{
		if(it ->second.userId.empty())
		{
			userToSessions_.erase(it->second.userId);
		}
		sessions_.erase(it);
	}
}

void SharedState::bindUser(const std::string& sessionId, const std::string& userId)
{
	std::lock_guard<std::mutex> lock(mutex_);
	
	auto it = sessions_.find(sessionId);
	if (it == sessions_.end())
	{
		return;
	}

	if (!it->second.userId.empty())
	{
		userToSessions_.erase(it->second.userId);
	}

	auto oldIt = userToSessions_.find(userId);
	if (oldIt != userToSessions_.end())
	{
		auto oldSessionIt = sessions_.find(oldIt->second);
		if (oldSessionIt != sessions_.end())
		{
			oldSessionIt->second.userId.clear();
		}
	}

	it->second.userId = std::move(userId);
	userToSessions_[it->second.userId] = sessionId;
}

void SharedState::broadcast(const std::string& message)
{
	std::vector<std::shared_ptr<WebSocketSession>> sessionsToSend;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		cleanupDeadSession();
		for (const auto& [id, data] : sessions_)
		{
			if (auto sp = data.session.lock())
			{
				sessionsToSend.push_back(sp);
			}
		}
	}
	for (const auto& session : sessionsToSend)
	{
		session->sendMessage(message);
	}
}

bool SharedState::sendMessageToSession(const std::string& sessionId, const std::string& message)
{

	std::shared_ptr<WebSocketSession> target;
	{
		std::lock_guard<std::mutex> lock(mutex_);

		auto it = sessions_.find(sessionId);
		if (it == sessions_.end())
		{
			return false;
		}

		target = it->second.session.lock();
		if (!target)
		{
			if (it->second.userId.empty())
			{
				userToSessions_.erase(it->second.userId);
			}
			sessions_.erase(it);
			return false;
		}
	}
	target->sendMessage(message);
	return true;
}

bool SharedState::sendMessageToUser(const std::string& userId, const std::string& message)
{
	std::shared_ptr<WebSocketSession> target;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		auto userIt = userToSessions_.find(userId);

		if (userIt == userToSessions_.end())
		{
			return false;
		}
		auto sessionIt = sessions_.find(userIt->second);
		if (sessionIt == sessions_.end())
		{
			userToSessions_.erase(userIt);
			return false;
		}
		target = sessionIt->second.session.lock();
		if (!target)
		{
			sessions_.erase(sessionIt);
			userToSessions_.erase(userIt);
			return false;
		}
	}
	target->sendMessage(message);
	return true;
}

bool SharedState::isOnline(const std::string& userId) const
{
	std::lock_guard<std::mutex> lock(mutex_);
	return userToSessions_.count(userId) > 0;
}

std::vector<std::string> SharedState::getOnlineUsers() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	std::vector<std::string> onlineUsers;
	onlineUsers.reserve(userToSessions_.size());

	for (const auto& [userId, sessionId] : userToSessions_)
	{
		auto it = sessions_.find(sessionId);
		if (it != sessions_.end() && !it->second.userId.empty())
		{
			onlineUsers.push_back(userId);
		}
	}
	return onlineUsers;
}

void SharedState::cleanupDeadSession()
{
	for(auto it = sessions_.begin(); it != sessions_.end();)
	{
		if (it->second.session.expired())
		{
			if (!it->second.userId.empty())
			{
				userToSessions_.erase(it->second.userId);
			}
			it = sessions_.erase(it);
		}
		else
		{
			++it;
		}
	}
}
