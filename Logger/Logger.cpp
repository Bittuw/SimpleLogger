// Logger.cpp: ���������� ���������������� ������� ��� ���������� DLL.
//

#include "stdafx.h"
#include <tchar.h>
#include <atomic>
#include <cassert>

#include "Logger.h"
#include "LoggerThread.h"


namespace LoggerThreadState {
	static std::atomic<bool> needWork = false;
	static std::atomic<bool> working = false;
	static std::atomic<bool> exit = false;

	std::shared_ptr<std::condition_variable> exitCV = std::make_shared<std::condition_variable>();
	Messages_buffer_ref ref_queue_buffer;
	Commands_buffer_ref ref_command_buffer;

	Destinations_list_ref ref_destinations;
	std::shared_ptr<std::mutex> ref_mutex;
}

Logger& Log = Logger::createInstance();

void Logger::start() {
	LoggingThread::startThread(_queue, _mutex, _wakeUp, _destination);
}

void Logger::stop() {
	LoggerThreadState::exit = true;
	_mutex->lock();
	_queue->push(std::make_shared<ExitMessage>(out_debug));
	_mutex->unlock();
}

Logger::Logger() :
	_queue(std::make_shared<Messages_queue>()),
	_mutex(std::make_shared<std::mutex>()),
	_wakeUp(std::make_shared<std::condition_variable>()),
	_destination(std::make_shared<Destinations_list>())
{  
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
	LoggerThreadState::ref_destinations = _destination;
	LoggerThreadState::ref_mutex = _mutex;

	std::ios_base::sync_with_stdio(false);

}

Logger& Logger::operator()(MessageTypes&& type) {
	_mutex->lock();
	typeMessage = type;
	return *this;
}

void Logger::operator<<(const char& str) { 
	auto message = std::make_shared<UserMessage>(std::to_string(str), typeMessage, (*_destination)[static_cast<int>(typeMessage)].out_dist);
	_queue->push(message);
	LoggerThreadState::needWork = true;
	_mutex->unlock();
	_wakeUp->notify_one();
}

void Logger::operator<<(const std::string& str) { 
	auto message = std::make_shared<UserMessage>(str, typeMessage, (*_destination)[static_cast<int>(typeMessage)].out_dist);
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

LoggingThread::LoggingThread(Messages_buffer_ref& queue, std::shared_ptr<std::mutex>& mutex, std::shared_ptr<std::condition_variable>& wakeUp, Destinations_list_ref& destination) :
	_queue(queue),
	_mutex(mutex),
	_wakeUp(wakeUp),
	_destination(destination),
	_buffer_commands(std::make_shared<std::vector<std::shared_ptr<BaseLoggerMessage>>>())
{
	LoggerThreadState::working = true;
	LoggerThreadState::ref_queue_buffer = queue;
	LoggerThreadState::ref_command_buffer = _buffer_commands;
}

void LoggingThread::startThread(Messages_buffer_ref& queue, std::shared_ptr<std::mutex>& mutex, std::shared_ptr<std::condition_variable>& wakeUp, std::shared_ptr<std::vector<Channal>>& destination) {
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

	std::atexit(LoggingThread::atexti_flush);

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

void LoggingThread::atexti_flush() {
	flush_command_buffer(LoggerThreadState::ref_command_buffer);
	flush_queue_buffer(LoggerThreadState::ref_queue_buffer);
	flush_destination_buffers(LoggerThreadState::ref_destinations);
	LoggerThreadState::working = false;
}

LoggingThread::~LoggingThread() {
	flush_command_buffer(_buffer_commands);
	flush_queue_buffer(_queue);
	flush_destination_buffers(_destination);
	LoggerThreadState::working = false;
}
