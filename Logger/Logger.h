// ����������� ���� ���� ifdef - ��� ����������� ����� �������� ��������, ���������� ��������� 
// �������� �� ��������� DLL. ��� ����� ������ DLL �������������� � �������������� ������� LOGGER_EXPORTS,
// � ��������� ������. ���� ������ �� ������ ���� ��������� � �����-���� �������
// ������������ ������ DLL. ��������� ����� ����� ������ ������, ��� �������� ����� �������� ������ ����, ����� 
// ������� LOGGER_API ��� ��������������� �� DLL, ����� ��� ������ DLL ����� �������,
// ������������ ������ ��������, ��� ����������������.
#ifdef LOGGER_EXPORTS	
#define LOGGER_API __declspec(dllexport)
#define EXPIMP_TEMPLATE 
#else
#define LOGGER_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif

#ifdef ERROR
#undef ERROR
#endif

#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <string>
#include <memory>
#include <mutex>
#include <queue>

#include "LoggerFormat.hpp"
#include "Data_types.hpp"

EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::mutex>;
EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::condition_variable>;
EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::ofstream>;
EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::vector<Channal>>;
EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::queue<std::shared_ptr<BaseLoggerMessage>>>;


class LOGGER_API Logger
{
public:

	~Logger();

	Logger& operator()(MessageTypes&& = MessageTypes::TRACE);
	void* operator new(size_t) = delete;
	void* operator new[](size_t) = delete;
	
	void start();
	void stop();

	void operator<<(const char&);
	void operator<<(const std::string&);
	static Logger createInstance();
	//static void onExit();

private:
	Logger();
	Logger(const Logger&) = default;

	Messages_buffer_ref _queue; // ������� ������
	std::shared_ptr<std::mutex> _mutex; // ������� �������
	std::shared_ptr<std::condition_variable> _wakeUp; // ��������� ����������

	MessageTypes typeMessage;

	

	static bool created;

	std::shared_ptr<std::ofstream> out_debug;
	std::shared_ptr<std::ofstream> out_error;
	std::shared_ptr<std::ofstream> out_event;

	Destinations_list_ref _destination;
	std::string path;
};

extern LOGGER_API Logger& Log;