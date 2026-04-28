#ifndef WEBSOCKETSESSION_H
#define WEBSOCKETSESSION_H
#include <string>

class WebSocketSession
{
public:
	bool sendMessage(const std::string& message);
};


#endif // !WEBSOCKETSESSION_H

