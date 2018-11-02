#include "Proxy/ProxyImpl.hpp"
#include "TLS/SimpleLoggerTLS.hpp"

namespace simplelogger::simpleproxy {

    ProxyImpl::ProxyImpl(simplemessage::Message message) :
        m_message(std::move(message))
    {

    }

    ProxyImpl::~ProxyImpl() {   

		/// TODO: Get from SimpleLogger_TLS from TLS Storage
		/// like get_instance thread_local, same as static
		/// push(Message)

    }

	void ProxyImpl::write(std::string_view str) {
		m_stream << str.data() << " ";
	}

	void ProxyImpl::flush() noexcept {
		m_message.m_message = std::move(m_stream.str());
		simplelogger::tls::SimpleLoggerTLS::get_thread_local_instance();
		/// blah flushing
	}
}