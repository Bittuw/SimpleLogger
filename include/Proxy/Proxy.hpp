#ifndef SIMPLE_PROXY
#define SIMPLE_PROXY

#include <string_view>
#include <string>
#include <memory>

#include "Type/Message.hpp"

namespace simplelogger::simpleproxy{

	class ProxyImpl;

    class Proxy {
    public:

		///
		/// Default deleter
		///
        ~Proxy();


		///
		/// Delete to prevent copy
		///
		Proxy(Proxy&) = delete;

		///
		/// Delete to prevent copy
		///
        Proxy& operator=(Proxy&&) = delete;

		///
		/// Delete to prevent copy
		///
        Proxy& operator=(Proxy&) = delete;

		///
		/// Friend function for catch base info to cunstruct Proxy and Message
		///
		friend Proxy log_impl(
			simplelogger::simplemessage::MessageType,
			std::string_view file,
			std::string_view func,
			int line
		);

		///
		/// Friend function to overload operator <<
		///
		friend Proxy&& operator<<(Proxy&&, std::string_view);

    private:

		/**
		 * @brief Construct a new Proxy object
		 * 
		 */
        Proxy(simplelogger::simplemessage::Message);

		/**
		 * @brief Construct a new Proxy object
		 * 
		 */
		Proxy(Proxy&&) = default;

        /**
         * @brief Main implementations of proxy
         * 
         */
        std::unique_ptr<ProxyImpl> m_impl;
	};

	/**
	 * @brief Operator, that help to reseive stringable type
	 * 
	 * @param proxy 
	 * @param view 
	 * @return Proxy&& 
	 */
	Proxy&& operator<<(Proxy&& proxy, std::string_view view);

	/**
	 * @brief Function, that create instance of Proxy
	 * 
	 * @param message_type 
	 * @param file 
	 * @param func 
	 * @param line 
	 * @return Proxy 
	 */
	Proxy log_impl(
		simplelogger::simplemessage::MessageType message_type,
		std::string_view file,
		std::string_view func,
		const int line
	);

/**
 * @brief Macro, to hide __FUNCTIONS__ and etc. macros
 * 
 */
#define SimpleLog(M_TYPE) simplelogger::simpleproxy::log_impl(M_TYPE, __FILE__, __FUNCTION__, __LINE__)

}

#endif