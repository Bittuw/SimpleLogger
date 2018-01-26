#pragma once

enum class ModeTypes {
	WriteDestinationOnly = 0,
	WriteDestinationAndConsole = 1
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
	USER_MESSAGE = 0,
	EXCEPTION_MESSAGE = 1,
	EXIT_MESSAGE = 2
};

static std::string BasicTypeMessageString[] = {
	"USER_MESSAGE",
	"EXCEPTION_MESSAGE",
	"EXIT_MESSAGE"
};

static std::string TypeMessageString[] = {
	"TRACE", // Âûגמה ג ךמםסמכü 
	"DEBUG", // Âûגמה ג פאיכ out_debug
	"WARNING", // Âûגמה ג פאיכ out_debug
	"ERROR", // Âûגמה ג פאכי out_error
	"FAILURE", // Âûגמה ג פאיכ out_error
	"EVENT" // Âûגמה ג פאיכ out_event
};

struct CurrentTime {
private:
	unsigned short tSecond;
	unsigned short tMinute;
	unsigned short tHour;
	unsigned short tDay;
	unsigned short tMonth;
	unsigned short tYear;

	std::string sSecond;
	std::string sMinute;
	std::string sHour;
	std::string sDay;
	std::string sMonth;
	std::string sYear;

public:
	std::string tCurrentTime;

	CurrentTime() {
		SYSTEMTIME currentTime;
		GetLocalTime(&currentTime);

		tSecond = currentTime.wSecond;
		tMinute = currentTime.wMinute;
		tHour = currentTime.wHour;
		tDay = currentTime.wDay;
		tMonth = currentTime.wMonth;
		tYear = currentTime.wYear;

		std::stringstream ss;
		ss << std::setfill('0') << std::setw(2) << std::right << tDay << ' ';
		ss << std::setfill('0') << std::setw(2) << std::right << tMonth << ' ';
		ss << std::setfill('0') << std::setw(4) << std::right << tYear << ' ';
		ss << std::setfill('0') << std::setw(2) << std::right << tHour << ' ';
		ss << std::setfill('0') << std::setw(2) << std::right << tMinute << ' ';
		ss << std::setfill('0') << std::setw(2) << std::right << tSecond << ' ';

		ss >> sDay >> sMonth >> sYear >> sHour >> sMinute >> sSecond;

		tCurrentTime = sDay + ':' + sMonth + ':' + sYear + ' ' + sHour + ':' + sMinute + ':' + sSecond;
	};
};

struct ChannalLine {
	std::string Name;
	std::shared_ptr<std::ostream> out_dist;
	ModeTypes mode;
};

typedef ChannalLine Channal;
////

struct BaseLoggerMessage {
	CommandTypes _message_type;
	CurrentTime _message_time;
	std::string _out_message;

	std::string _time_string;
	std::string _type_string;

	bool _message_delivered = false;

	BaseLoggerMessage(CommandTypes message_type) 
		: _message_type(message_type), _message_time() { basic_format(); }

	virtual ~BaseLoggerMessage() {}
	virtual void work() {};
	virtual void format() {};

	void basic_format() {
		std::stringstream ss;

		std::string temp_type;
		std::string temp_time;
		std::string temp_data;

		ss << std::setw(17) << BasicTypeMessageString[static_cast<int>(_message_type)] << ' ';
		ss << _message_time.tCurrentTime;
		ss >> temp_type >> temp_data >> temp_time;

		_time_string = temp_data + ' ' + temp_time;
		_type_string = temp_type;
	};
};

struct UserMessage : public BaseLoggerMessage {
	std::string _user_message;
	MessageTypes _type_message;
	std::shared_ptr<std::ostream> _destination;

	UserMessage(std::string user_message, MessageTypes type_message, std::shared_ptr<std::ostream> destination) :
		BaseLoggerMessage(CommandTypes::USER_MESSAGE), _user_message(user_message), _type_message(type_message), _destination(destination){
		format();
	}

	void work() override {
		_destination->clear();
		*_destination << _out_message;
		_message_delivered = true;
	}

	void format() {
		std::stringstream ss;

		std::string temp_type;

		ss << std::setw(7) << TypeMessageString[static_cast<int>(_type_message)] << ' ';
		ss >> temp_type;

		_out_message = _type_string + " :: " +  temp_type + " || " + _time_string + " || " + _user_message + "\n";
	}
};

struct ExceptionMessage : public BaseLoggerMessage {
	std::shared_ptr<std::vector<Channal>> _destination;

	ExceptionMessage(std::shared_ptr<std::vector<Channal>> destination) :
		BaseLoggerMessage(CommandTypes::EXCEPTION_MESSAGE), _destination(destination){	}

	void work() override {
		for (auto dist : *_destination) {
			std::dynamic_pointer_cast<std::ofstream>(dist.out_dist)->close();
		}
	}

	void format() override { // TODO override

	}
};

struct ExitMessage : public BaseLoggerMessage {
	
	std::shared_ptr<std::ostream> _destination;
	const std::string _out_ = "Successful exit";

	ExitMessage(std::shared_ptr<std::ostream> destination) :
		BaseLoggerMessage(CommandTypes::EXIT_MESSAGE), _destination(destination) { format(); }

	void work() override {
		*_destination << _out_message;
	}

	void format() override {
		std::stringstream ss;

		_out_message = _type_string + " || " + _time_string + " || " + _out_;
	}
};

using Destinations_list = std::vector<Channal>;
using Messages_queue = std::queue<std::shared_ptr<BaseLoggerMessage>>;

using Destinations_list_ref = std::shared_ptr<std::vector<Channal>>;
using Messages_buffer_ref = std::shared_ptr<std::queue<std::shared_ptr<BaseLoggerMessage>>>;
using Commands_buffer_ref = std::shared_ptr<std::vector<std::shared_ptr<BaseLoggerMessage>>>;

