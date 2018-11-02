#include "TLS/SimpleLoggerTLS.hpp"
#include "TLS/SimpleLoggerTLSImpl.hpp"

namespace simplelogger::tls {
    SimpleLoggerTLS::SimpleLoggerTLS() noexcept
        :
            m_impl(std::make_unique<SimpleLoggerTLSImpl>())
    {
        m_impl->init_failure_capture();
    }

	SimpleLoggerTLS::~SimpleLoggerTLS() noexcept {

	}

    SimpleLoggerTLS& SimpleLoggerTLS::get_thread_local_instance() noexcept {
        thread_local SimpleLoggerTLS m_tls{};
        return m_tls;
    }
}