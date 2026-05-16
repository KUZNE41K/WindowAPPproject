#include "ConnectionPool.h"

ConnectionPool::ConnectionPool(const std::string& conn_string, int max_conn)
	:max_connections_(max_conn),
	connectonString_(conn_string),
	isShutDown_(false)
{
	for (int i = 0; i < max_connections_; i++)
	{
		auto conn = createConnection();
		if (conn && conn->is_open())
		{
			pool_.push(conn);
			std::cout << "  Connection " << (i + 1) << "/" << max_connections_ << " created successfully" << std::endl;
		}
		else
		{
			std::cerr << "  Failed to create connection " << (i + 1) << "/" << max_connections_ << std::endl;
		}
	}
	std::cout << "Connection pool ready. Available connections: " << pool_.size() << std::endl;

}

ConnectionPool::~ConnectionPool()
{
	shutdown();
}
std::shared_ptr<pqxx::connection> ConnectionPool::get()
{
	std::unique_lock<std::mutex> lock(mutex_);
	cv_.wait(lock, [this] {
		return !pool_.empty() || isShutDown_;
		});

	if (isShutDown_ && pool_.empty())
	{
		throw std::runtime_error("Connection pool is shutdown and no connections available");
	}

	auto conn = pool_.front();
	pool_.pop();

	if (!conn || !conn->is_open())
	{
		lock.unlock();
		conn = createConnection();
		lock.lock();

		if (!conn || !conn->is_open())
		{
			throw std::runtime_error("Failed to create new database connection");
		}
	}
	return conn;
}

void ConnectionPool::put(std::shared_ptr<pqxx::connection> conn)
{
	if (!conn || !conn->is_open())
	{
		std::cerr << "Warning: Attempting to return dead connection to pool" << std::endl;
		return;
	}
	std::lock_guard<std::mutex> lock(mutex_);

	if (!isShutDown_)
	{
		pool_.push(conn);
		cv_.notify_one();
	}
}


size_t ConnectionPool::size() const
{
	std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mutex_));
	return pool_.size();
}

bool ConnectionPool::empty() const
{
	std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mutex_));
	return pool_.empty();

}
void ConnectionPool::shutdown()
{
	std::lock_guard<std::mutex> lock(mutex_);
	isShutDown_ = true;

	while (!pool_.empty())
	{
		pool_.pop();
	}
	cv_.notify_all();
	std::cout << "Connection pool shutdown complete" << std::endl;
}

std::shared_ptr<pqxx::connection> ConnectionPool::createConnection()
{
	try
	{
		auto conn = std::make_shared<pqxx::connection>(connectonString_);

		if (conn && conn->is_open())
		{
			return conn;
		}
	}
	catch(std::exception&e)
	{
		std::cerr << "Connection creation error: " << e.what() << std::endl;
	}
	return nullptr;
}
