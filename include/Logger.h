#ifndef SIMPLELOGGER
#define SIMPLELOGGER

#ifdef simplelogger_EXPORTS	
#define LOGGER_API __declspec(dllexport)
#define EXPIMP_TEMPLATE 
#else
#define LOGGER_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif

#ifdef ERROR
#undef ERROR
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <mutex>

#include "Types.hpp"

namespace SimpleLogger {

	EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::mutex>;
	EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::condition_variable>;
	EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::wofstream>;
	EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::vector<Channal>>;
	EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::queue<std::shared_ptr<BaseLoggerMessage>>>;


	class LOGGER_API Logger
	{
	public:

		~Logger();

		Logger& operator()(Message_Types&&);
		Logger& operator()();
		void* operator new(size_t) = delete;
		void* operator new[](size_t) = delete;
		
		void start();
		void stop();

		void operator<<(const wchar_t*);
		void operator<<(const std::wstring&);
		static Logger createInstance();
		//static void onExit();

	private:
		Logger();
		Logger(const Logger&) = default;

		Messages_buffer_ref _queue; // 
		std::shared_ptr<std::mutex> _mutex; // 
		std::shared_ptr<std::condition_variable> _wakeUp; // 

		Message_Types typeMessage;

		static bool created;

		std::shared_ptr<std::wofstream> out_debug;
		std::shared_ptr<std::wofstream> out_error;
		std::shared_ptr<std::wofstream> out_event;

		Destinations_list_ref _destination;
		std::wstring path;
	};

	extern LOGGER_API Logger& Log;
}
#endif