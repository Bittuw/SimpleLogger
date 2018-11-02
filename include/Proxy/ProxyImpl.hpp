#ifndef SIMPLE_PROXY_IMPL
#define SIMPLE_PROXY_IMPL

#include <sstream>

#include "Type/Message.hpp"


namespace simpleloggertls {
    class SimpleLogger_TLS;
}

namespace simplelogger::simpleproxy {

    class ProxyImpl /*: public ProxyInterface<ProxyImpl>*/ {
    public:
        ProxyImpl(simplelogger::simplemessage::Message); 
        ~ProxyImpl(); /* Auto flush after destruct
            Здесь хранится сам инстанс SimpleLogger_TLS, который 
            непослердственно представляет точку входа для всего потока.
        */
		ProxyImpl(ProxyImpl&&) = default;
		void write(std::string_view);
		void flush() noexcept;

    private:
        std::stringstream m_stream;
        simplemessage::Message m_message;
    };
}

#endif