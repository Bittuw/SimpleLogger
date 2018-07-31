#pragma once

#include <Windows.h>
#include <sstream>
#include <iomanip>
#include <queue>

#undef ERROR

namespace SimpleLogger {
	
	enum class ModeTypes {
		WriteDestinationOnly = 0,
		WriteDestinationAndConsole = 1
	};

	enum class Message_Types {
		TRACE = 0,
		DEBUG = 1,
		WARNING = 2,
		ERROR = 3,
		FAILURE = 4,
		EVENT = 5
	};

	enum class CommandTypes {
		USER_MESSAGE = 0,
		EXCEPTION_MESSAGE = 1,
		EXIT_MESSAGE = 2
	};

	static std::wstring BasicTypeMessageString[] = {
		L"USER_MESSAGE",
		L"EXCEPTION_MESSAGE",
		L"EXIT_MESSAGE"
	};

	static std::wstring TypeMessageString[] = {
		L"TRACE", // Вывод в консоль 
		L"DEBUG", // Вывод в файл out_debug
		L"WARNING", // Вывод в файл out_debug
		L"ERROR", // Вывод в фалй out_error
		L"FAILURE", // Вывод в файл out_error
		L"EVENT" // Вывод в файл out_event
	};

	struct CurrentTime {
	private:
		unsigned short tSecond;
		unsigned short tMinute;
		unsigned short tHour;
		unsigned short tDay;
		unsigned short tMonth;
		unsigned short tYear;

		std::wstring sSecond;
		std::wstring sMinute;
		std::wstring sHour;
		std::wstring sDay;
		std::wstring sMonth;
		std::wstring sYear;

	public:
		std::wstring tCurrentTime;

		CurrentTime() {
			SYSTEMTIME currentTime;
			GetLocalTime(&currentTime);

			tSecond = currentTime.wSecond;
			tMinute = currentTime.wMinute;
			tHour = currentTime.wHour;
			tDay = currentTime.wDay;
			tMonth = currentTime.wMonth;
			tYear = currentTime.wYear;

			std::wstringstream ss;
			ss << std::setfill(L'0') << std::setw(2) << std::right << tDay << L' ';
			ss << std::setfill(L'0') << std::setw(2) << std::right << tMonth << L' ';
			ss << std::setfill(L'0') << std::setw(4) << std::right << tYear << L' ';
			ss << std::setfill(L'0') << std::setw(2) << std::right << tHour << L' ';
			ss << std::setfill(L'0') << std::setw(2) << std::right << tMinute << L' ';
			ss << std::setfill(L'0') << std::setw(2) << std::right << tSecond << L' ';

			ss >> sDay >> sMonth >> sYear >> sHour >> sMinute >> sSecond;

			tCurrentTime = sDay + L':' + sMonth + L':' + sYear + L' ' + sHour + L':' + sMinute + L':' + sSecond;
		};
	};

	struct ChannalLine {
		std::wstring Name;
		std::shared_ptr<std::wostream> out_dist;
		ModeTypes mode;
	};

	typedef ChannalLine Channal;
	////

	struct BaseLoggerMessage {
		CommandTypes _message_type;
		CurrentTime _message_time;
		std::wstring _out_message;

		std::wstring _time_string;
		std::wstring _type_string;

		bool _message_delivered = false;

		BaseLoggerMessage(CommandTypes message_type) 
			: _message_type(message_type), _message_time() { basic_format(); }

		virtual ~BaseLoggerMessage() {}
		virtual void work() {};
		virtual void format() {};

		void basic_format() {
			std::wstringstream ss;

			std::wstring temp_type;
			std::wstring temp_time;
			std::wstring temp_data;

			ss << std::setw(17) << BasicTypeMessageString[static_cast<int>(_message_type)] << L' ';
			ss << _message_time.tCurrentTime;
			ss >> temp_type >> temp_data >> temp_time;

			_time_string = temp_data + L' ' + temp_time;
			_type_string = temp_type;
		};
	};

	struct UserMessage : public BaseLoggerMessage {
		std::wstring _user_message;
		Message_Types _type_message;
		ModeTypes _mode;
		std::shared_ptr<std::wostream> _destination;

		UserMessage(std::wstring user_message, Message_Types type_message, std::shared_ptr<std::wostream> destination, ModeTypes mode) :
			BaseLoggerMessage(CommandTypes::USER_MESSAGE), _user_message(user_message), _type_message(type_message), _destination(destination), _mode(mode){
			format();
		}

		void work() override {
			_destination->clear();
			*_destination << _out_message;
			if (_mode == ModeTypes::WriteDestinationAndConsole) std::wcout << _out_message;
			_message_delivered = true;
		}

		void format() {
			std::wstringstream ss;

			std::wstring temp_type;

			ss << std::setw(7) << TypeMessageString[static_cast<int>(_type_message)] << L' ';
			ss >> temp_type;

			_out_message = _type_string + L" :: " +  temp_type + L" || " + _time_string + L" || " + _user_message + L"\n";
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
		
		std::shared_ptr<std::wostream> _destination;
		const std::wstring _out_ = L"Successful exit";

		ExitMessage(std::shared_ptr<std::wostream> destination) :
			BaseLoggerMessage(CommandTypes::EXIT_MESSAGE), _destination(destination) { format(); }

		void work() override {
			*_destination << _out_message;
		}

		void format() override {
			std::wstringstream ss;

			_out_message = _type_string + L" || " + _time_string + L" || " + _out_;
		}
	};

	using Destinations_list = std::vector<Channal>;
	using Messages_queue = std::queue<std::shared_ptr<BaseLoggerMessage>>;

	using Destinations_list_ref = std::shared_ptr<std::vector<Channal>>;
	using Messages_buffer_ref = std::shared_ptr<std::queue<std::shared_ptr<BaseLoggerMessage>>>;
	using Commands_buffer_ref = std::shared_ptr<std::vector<std::shared_ptr<BaseLoggerMessage>>>;
}
