// Logger.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "Logger.h"
#include "LoggerThread.h"
#include <atomic>
#include <cassert>

namespace LoggerThreadState {
	static std::atomic<bool> needWork = false;
	static std::atomic<bool> working = false;
	static std::atomic<bool> exit = false;

	std::shared_ptr<std::condition_variable> exitCV = std::make_shared<std::condition_variable>();
	std::shared_ptr<MessageQueue> ref_queue;
	Buffer_ref ref_buffer;
	std::shared_ptr<std::vector<Channal>> ref_flush;
	std::shared_ptr<std::mutex> ref_mutex;
}

Logger& Log = Logger::createInstance();

void Logger::start() {
	LoggingThread::startThread(_queue, _mutex, _wakeUp, _destination);
}

Logger::Logger() :
	_queue(std::make_shared<MessageQueue>()),
	_mutex(std::make_shared<std::mutex>()),
	_wakeUp(std::make_shared<std::condition_variable>()),
	_destination(std::make_shared<std::vector<Channal>>())
{  
	//LoggingThread::startThread(_queue, _mutex, _wakeUp, _destination);

	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, sizeof(buffer));
	std::wstring buffer_w(buffer);
	auto pos = buffer_w.find_last_of(L"\\");
	buffer_w = buffer_w.substr(0, pos) + L"\\Log";
	path = std::string(buffer_w.begin(), buffer_w.end());

	_tmkdir(buffer_w.c_str());

	out_debug = std::make_shared<std::ofstream>(path + std::string("\\DLog.log"), std::ofstream::ate | std::ofstream::app);

	out_error = std::make_shared<std::ofstream>(path + std::string("\\ERLog.log"), std::ofstream::ate | std::ofstream::app);

	out_event = std::make_shared<std::ofstream>(path + std::string("\\EVLog.log"), std::ofstream::ate | std::ofstream::app);

	*_destination = {
		{ TypeMessageString[static_cast<int>(MessageTypes::TRACE)], out_debug, ModeTypes::WriteDestinationAndConsole },
		{ TypeMessageString[static_cast<int>(MessageTypes::DEBUG)], out_debug, ModeTypes::WriteDestinationAndConsole },
		{ TypeMessageString[static_cast<int>(MessageTypes::WARNING)], out_debug, ModeTypes::WriteDestinationAndConsole },
		{ TypeMessageString[static_cast<int>(MessageTypes::ERR)], out_error, ModeTypes::WriteDestinationAndConsole },
		{ TypeMessageString[static_cast<int>(MessageTypes::FAILURE)], out_error, ModeTypes::WriteDestinationAndConsole },
		{ TypeMessageString[static_cast<int>(MessageTypes::EVENT)], out_event, ModeTypes::WriteDestinationAndConsole }
	};

	// For flush on Exit
	LoggerThreadState::ref_flush = _destination;
	LoggerThreadState::ref_mutex = _mutex;

	std::ios_base::sync_with_stdio(false);

	//const int result = std::atexit(Logger::onExit);
}
//
//void Logger::onExit() {
//	LoggerThreadState::exit = true;
//}

Logger& Logger::operator()(MessageTypes&& type) {
	_mutex->lock();
	typeMessage = type;
	return *this;
}

void Logger::operator<<(const char& str) { 
	auto message = std::make_shared<UserMessage>(std::to_string(str), typeMessage, (*_destination)[static_cast<int>(typeMessage)].out_dist);
	//std::dynamic_pointer_cast<BaseLoggerMessage>(message);
	_queue->push(message);
	LoggerThreadState::needWork = true;
	_mutex->unlock();
	_wakeUp->notify_one();
}

void Logger::operator<<(const std::string& str) { 
	auto message = std::make_shared<UserMessage>(str, typeMessage, (*_destination)[static_cast<int>(typeMessage)].out_dist);
	//std::dynamic_pointer_cast<BaseLoggerMessage>(message);
	_queue->push(message);
	LoggerThreadState::needWork = true;
	_mutex->unlock();
	_wakeUp->notify_one();
}


Logger::~Logger() {}

bool Logger::created = false;
std::mutex mutex;

Logger Logger::createInstance() {
	if (!created) {
		created = true;
		return Logger();
	}
	else {
		assert(false);
	}
}

bool LoggingThread::created = false;

LoggingThread::LoggingThread(std::shared_ptr<MessageQueue>& queue, std::shared_ptr<std::mutex>& mutex, std::shared_ptr<std::condition_variable>& wakeUp, std::shared_ptr<std::vector<Channal>>& destination) :
	_queue(queue),
	_mutex(mutex),
	_wakeUp(wakeUp),
	_destination(destination),
	_buffer_commands(std::make_shared<std::vector<std::shared_ptr<BaseLoggerMessage>>>())
{
	LoggerThreadState::working = true;
	LoggerThreadState::ref_queue = queue;
	LoggerThreadState::ref_buffer = _buffer_commands;
}

void LoggingThread::startThread(std::shared_ptr<MessageQueue>& queue, std::shared_ptr<std::mutex>& mutex, std::shared_ptr<std::condition_variable>& wakeUp, std::shared_ptr<std::vector<Channal>>& destination) {
	if (!created) {
		std::thread LoggingThread(&LoggingThread::RunningInThread, new LoggingThread(queue, mutex, wakeUp, destination));
		LoggingThread.detach();
		created = true;
	}
	else {
		Log(MessageTypes::ERR) << std::string("Second attempt to create a class \"Logger\"!");
	}
}

void* LoggingThread::operator new(size_t size) {
	return malloc(size);
}

void LoggingThread::operator delete(void* pointer) {
	delete pointer;
}

void LoggingThread::RunningInThread() {

	std::atexit(LoggingThread::flush);

	while (!LoggerThreadState::exit) {

		std::unique_lock<std::mutex> lock(*_mutex);
		_wakeUp->wait(lock, []() { return LoggerThreadState::needWork == true; });

		LoggerThreadState::needWork = false;

		while (!_queue->empty()) {
			_buffer_commands->push_back(_queue->front());
			_queue->pop();
		}

		lock.unlock();

		for (auto command : *_buffer_commands) {
			command->work();
		}

		_buffer_commands->clear();
	}
}

void LoggingThread::flush() {
	try {

		if (!LoggerThreadState::ref_buffer->empty())
			for (auto command : *(LoggerThreadState::ref_buffer))
				if (!command->_message_delivered)
					command->work();

		LoggerThreadState::ref_buffer->clear();

		if (!LoggerThreadState::ref_queue->empty())
			while (!LoggerThreadState::ref_queue->empty()) {
				auto command = LoggerThreadState::ref_queue->front();
				LoggerThreadState::ref_queue->pop();
				command->work();
			}

		

		for (auto dist : *LoggerThreadState::ref_flush) {
			dist.out_dist->flush();
			std::dynamic_pointer_cast<std::ofstream>(dist.out_dist)->close();
		}
	}
	catch (...) {
		auto i = 0;
	}
}
