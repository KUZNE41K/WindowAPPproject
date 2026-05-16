#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <iostream>
#include <pqxx/pqxx>

class ConnectionPool
{
public:
	ConnectionPool(const std::string& conn_string,int max_conn = 20);
	~ConnectionPool();

    std::shared_ptr<pqxx::connection> get();
    void put(std::shared_ptr<pqxx::connection> conn);

	size_t size() const;

    bool empty() const;

    void shutdown();



private:
	std::shared_ptr<pqxx::connection> createConnection();

	std::queue<std::shared_ptr<pqxx::connection>> pool_;
	std::mutex mutex_;
	std::condition_variable cv_;
	int max_connections_;
	std::string connectonString_;
	bool isShutDown_;

};
#endif // !CONNECTION_POOL_H

