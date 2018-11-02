#ifndef SIMPLE_LOGGER_FAILURE_FINCTIONS
#define SIMPLE_LOGGER_FAILURE_FINCTIONS

#include "Utils/Exceptions.hpp"

#ifdef _WIN32
// NOTE:  for windows exception include

#include <Windows.h>

#define GENERATE_CASE(WINCODE, MSG) case WINCODE: {\
    throw simplelogger::exceptions::platformspec::INTERNAL_##WINCODE(exception, MSG);\
    break;\
}
#elif LINUX
    // NOTE:  for linux exception include
#endif

/**
 * @brief Platform spec translators
 * 
 */
namespace simplelogger::exceptions::failurefunctions {
#ifdef _WIN32 || _WIN64

    /**
     * @brief Function, that translate WINDOWS platform depended exception
     * 
     * @param u 
     * @param exception 
     */
    void translate_platform_exception(unsigned int u, PEXCEPTION_POINTERS exception) {
        switch(exception->ExceptionRecord->ExceptionCode) {
            GENERATE_CASE(EXCEPTION_ACCESS_VIOLATION, "Access violation")
            GENERATE_CASE(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, "Element is out of bound")
            GENERATE_CASE(EXCEPTION_DATATYPE_MISALIGNMENT, "Bad aligment")
            GENERATE_CASE(EXCEPTION_FLT_DENORMAL_OPERAND, "Denormal operands (float)")
            GENERATE_CASE(EXCEPTION_FLT_DIVIDE_BY_ZERO, "Divided by zero (float)")
            GENERATE_CASE(EXCEPTION_FLT_INEXACT_RESULT, "Inexact result (float)")
            GENERATE_CASE(EXCEPTION_FLT_INVALID_OPERATION, "Invalid operation (float)")
            GENERATE_CASE(EXCEPTION_FLT_OVERFLOW, "Overflow (float)")
            GENERATE_CASE(EXCEPTION_FLT_STACK_CHECK, "Stack overflow (float)")
            GENERATE_CASE(EXCEPTION_FLT_UNDERFLOW, "Undeflow (float)")
            GENERATE_CASE(EXCEPTION_ILLEGAL_INSTRUCTION, "Illegal instruction")
            GENERATE_CASE(EXCEPTION_IN_PAGE_ERROR, "Access page error")
            GENERATE_CASE(EXCEPTION_INT_DIVIDE_BY_ZERO, "Divided by zero (int)")
            GENERATE_CASE(EXCEPTION_INT_OVERFLOW, "Overflow (int)")
            GENERATE_CASE(EXCEPTION_INVALID_DISPOSITION, "Invalid exception dispasition")
            GENERATE_CASE(EXCEPTION_NONCONTINUABLE_EXCEPTION, "Noncontinuable exception")
            GENERATE_CASE(EXCEPTION_PRIV_INSTRUCTION, "Not allowed operation")
            GENERATE_CASE(EXCEPTION_STACK_OVERFLOW, "Stakc overflow (thread)")
            default: {
                break;
            }
        }
    }
#elif _linux_

#endif
}
#endif