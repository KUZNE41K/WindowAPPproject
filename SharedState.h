#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class WebSocketSession;

class SharedState
{
public:

	explicit SharedState(std::string docRoot);
	~SharedState() = default;

	SharedState(const SharedState&) = delete;
	SharedState& operator=(const SharedState&) = delete;
	SharedState(const SharedState&&) = delete;
	SharedState& operator=(const SharedState&&) = delete;

	void join(const std::string& sessionId,std::shared_ptr<WebSocketSession> sessions);
	void leave(const std::string& sessionId);

	void bindUser(const std::string& sessionId, const std::string& userId);

	void broadcast(const std::string& message);
	bool sendMessageToSession(const std::string& sessionId, const std::string& message);
	bool sendMessageToUser(const std::string& userId, const std::string& message);

	bool isOnline(const std::string& userId) const;
	std::vector<std::string> getOnlineUsers() const;

	const std::string& getDocRoot()const { return docRoot_; }

private:
	struct SessionData
	{
		std::weak_ptr<WebSocketSession> session;
		std::string userId;
	};
	void cleanupDeadSession();

	std::string docRoot_;
	mutable std::mutex mutex_;

	std::unordered_map<std::string, SessionData> sessions_;
	std::unordered_map<std::string, std::string> userToSessions_;

};




#endif // !SHAREDSTATE_H
