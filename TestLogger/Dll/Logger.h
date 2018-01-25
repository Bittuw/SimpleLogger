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

enum class ModeTypes {
	WriteConsoleOnly = 0,
	WriteDestinationOnly = 1,
	WriteDestinationAndConsole = 2
};

enum class MessageTypes {
	TRACE = 0,
	DEBUG = 1,
	WARNING = 2,
	ERR = 3,
	FAILURE = 4,
	EVENT = 5
};

enum class CommandTypes {
	UNDEFINE = 0,
	STANDART_MESSAGE = 1,
	EXCEPTION_MESSAGE = 2,
	EXIT = 3
};

EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::mutex>;
EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<MessageQueue>;
EXPIMP_TEMPLATE template class LOGGER_API std::shared_ptr<std::condition_variable>;

class LOGGER_API Logger
{
public:

	~Logger();

	Logger& operator()(MessageTypes&& = MessageTypes::TRACE);
	void* operator new(size_t) = delete;
	void operator<<(const char&);
	void operator<<(const std::string&);
	static Logger createInstance();

private:
	Logger();

	std::shared_ptr<MessageQueue> _queue;
	std::shared_ptr<std::mutex> _mutex; // ������� �������
	std::shared_ptr<std::condition_variable> _wakeUp;
	MessageTypes typeMessage;

	Logger(const Logger&);

	static bool created;
};

extern LOGGER_API Logger& Log;