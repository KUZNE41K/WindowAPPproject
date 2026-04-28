#include "SharedState.h"
#include "WebSocketSession.h"
#include <algorithm>
SharedState::SharedState(std::string docRoot): docRoot_(std::move(docRoot))
{
}

void SharedState::join(std::shared_ptr<WebSocketSession> sessions)
{
	std::lock_guard<std::mutex> lock(mutex_);
	cleanupDeadSession();

	sessions_.insert(std::weak_ptr<WebSocketSession>(sessions));



}

void SharedState::leave(std::shared_ptr<WebSocketSession> webSocketSession)
{
}

void SharedState::broadcast(const std::string& message)
{
}

void SharedState::getDocRoot() const
{
}

void SharedState::cleanupDeadSession()
{
}
