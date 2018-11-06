#include "TLS/SImpleLoggerTLSImpl.hpp"
#include "Utils/FailureFunctions.hpp"

namespace simplelogger::tls {
    SimpleLoggerTLSImpl::SimpleLoggerTLSImpl() noexcept {
        
    }

    SimpleLoggerTLSImpl::~SimpleLoggerTLSImpl() noexcept {

    }

    void SimpleLoggerTLSImpl::init_failure_capture() noexcept {
// #ifdef _WIN32
//         auto lek = _set_se_translator(simplelogger::exceptions::failurefunctions::translate_platform_exception);
// #elif LINUX
    // TODO: create linux implement of init_failure_capture function
// #endif
    }
}