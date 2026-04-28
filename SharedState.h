#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H
#include <string>
#include <memory>
#include <mutex>
#include <unordered_set>

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

	void join(std::shared_ptr<WebSocketSession> webSocketSession);

	void leave(std::shared_ptr<WebSocketSession>webSocketSession);
	
	void broadcast(const std::string& message);

	void getDocRoot()const;



private:
	void cleanupDeadSession();

	std::string docRoot_;
	std::mutex mutex_;

	std::unordered_set<std::weak_ptr<WebSocketSession>> sessions_;
};




#endif // !SHAREDSTATE_H
