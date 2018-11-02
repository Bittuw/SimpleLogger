#ifndef SIMPLE_LOGGER_MESSAGE
#define SIMPLE_LOGGER_MESSAGE


#include <string_view>
#include <iostream>
#include <string>
#include <chrono>
#include <array>

#include "Utils/Operators.inl"

#ifdef ERROR
#undef ERROR
#endif

/**
 * @brief Message scope
 * 
 */
namespace simplelogger::simplemessage {

    /**
     * @brief 
     * 
     */
    using time_point = std::chrono::time_point<std::chrono::steady_clock>;
    

    /**
     * @brief Enum class, that contains message layers
     * 
     */
    enum class MessageType {
		TRACE = 0,
		DEBUG = 1,
        INFO = 2,
		WARNING = 3,
		ERROR = 4,
		FAILURE = 5,
        CRASH = 6
	};

    /**
     * @brief string presenter of layers
     * 
     */
    constexpr const std::array<std::string_view, 7> array = {
        "TRACE",
        "DEBUG",
        "INFO",
        "WARNING",
        "ERROR",
        "FAILURE",
        "CRASH",
    }; 


    /**
     * @brief Transport message, this struct using to create 
     * 
     */
    struct Message : simpleoperators::Operators<Message> {

        /**
         * @brief Layer of logging
         * 
         */
        MessageType m_message_type;
 
        /**
         * @brief File name
         * 
         */
        std::string m_file;

        /**
         * @brief Fcuntion name
         * 
         */
        std::string m_func;

        /**
         * @brief Line number
         * 
         */
        int m_line;

        /**
         * @brief User message
         * 
         */
        std::string m_message;

        /**
         * @brief STL time point
         * 
         */
        time_point m_message_time_point; 

        /**
         * @brief Help to implement all compare operators
         * 
         * @param left 
         * @param right 
         * @return true 
         * @return false 
         */
		friend bool operator<(const Message& left, const Message& right) noexcept {
			return left.m_message_time_point < right.m_message_time_point;
		}
    };
}

#endif