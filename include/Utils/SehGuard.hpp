#ifndef SIMPLE_LOGGER_SEH_GUARD
#define SIMPLE_LOGGER_SEH_GUARD

#include <Windows.h>

#include "Utils/FailureFunctions.hpp"



namespace simplelogger::utils {
    struct SehGuard {
        using filter = _se_translator_function;

        SehGuard() {
            m_prev = _set_se_translator(
                simplelogger::exceptions::failurefunctions::translate_platform_exception
            );
        }

        ~SehGuard() {
            _set_se_translator(m_prev);
        }

        filter m_prev;
    };
}

#endif