#pragma once

class LoggingThread {
public:
	LoggingThread(std::shared_ptr<MessageQueue>&, std::shared_ptr<std::mutex>&, std::shared_ptr<std::condition_variable>&, std::shared_ptr<std::vector<Channal>>&);
	static void startThread(std::shared_ptr<MessageQueue>&, std::shared_ptr<std::mutex>&, std::shared_ptr<std::condition_variable>&, std::shared_ptr<std::vector<Channal>>&);
	LoggingThread(const LoggingThread&) = delete;
	void* operator new[](size_t) = delete;
	void operator delete[](void*) = delete;
	void operator delete(void* pointer);

private:
	explicit LoggingThread() {};
	void* operator new(size_t);

	std::shared_ptr<MessageQueue> _queue;
	std::shared_ptr<std::mutex> _mutex;
	std::shared_ptr<std::condition_variable> _wakeUp;

	std::shared_ptr<std::vector<Channal>> _destination;

	void RunningInThread();

	static bool created;
};