#include <string>
#ifndef THREADS_H
#define THREADS_H



class Threads
{
public:
    Threads();
    Threads(const std::string& title, const std::string& uuid, const std::string& createdId);

	std::string getTitle() const;
	std::string getUuid() const;
	std::string getCreatedId() const;

	void setTitle(const std::string& title);
	void setUuid(const std::string& uuid);
	void setCreatedId(const std::string& createdId);




private:
	std::string title_;
	std::string uuid_;
	std::string createdId_;


};
#endif // !THREADS_H

