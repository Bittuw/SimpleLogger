#ifndef SIMPLE_LOGGER_EXCEPTIONS
#define SIMPLE_LOGGER_EXCEPTIONS
#include <string_view>
#include <exception>
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#elif __linux__

#endif

#include "Proxy/Proxy.hpp"
#include "Utils/Format.hpp"

namespace simplelogger::exceptions {

/* TODO: linux class  */

#define GENERATE_WIN_EXCEPTION_CLASS(WINCODE)\
struct INTERNAL_##WINCODE : public platform::WIN_EXCEPTION {\
    using platform::WIN_EXCEPTION::WIN_EXCEPTION;\
};

    /**
     * @brief Basic exception for SimpleLogger
     * 
     */
    namespace basic {
        struct SimpleException : public std::exception {
        public:
        /**
         * @brief Construct a new Simple Exception object
         * 
         * @param str 
         */
            explicit SimpleException(std::string_view str) noexcept
                :
                    m_msg(std::make_unique<std::string>(str)) // prevent throw std::string exception
            {}

            /**
             * @brief Destroy the Simple Exception object
             * 
             */
            virtual ~SimpleException() noexcept {};

            /**
             * @brief 
             * 
             * @return const char* 
             */
            [[deprecated("stl style const char* type!")]] virtual const char* what() const noexcept {
                return m_msg.get()->data();
            } 

            /**
             * @brief 
             * 
             * @return std::string_view 
             */
            virtual std::string_view what_advance() const noexcept {
                return { m_msg.get()->data() };
            }

        protected:

            /**
             * @brief final User message
             * 
             */
            std::unique_ptr<std::string> m_msg;
        }; 
    }

    /**
     * @brief Platform main exception structs
     * 
     */
    namespace platform {
#ifdef _WIN32

        /**
         * @brief WINDOWS depended class
         * 
         */
        struct WIN_EXCEPTION : public basic::SimpleException {

            /**
             * @brief Construct a new win exception object
             * 
             * @param point 
             * @param message 
             */
            explicit WIN_EXCEPTION(PEXCEPTION_POINTERS point, std::string_view message) : SimpleException(
                    simplelogger::utils::format("Message: %, code: %, address: %", 
                        message,
                        point->ExceptionRecord->ExceptionCode,
                        point->ExceptionRecord->ExceptionAddress
                    )
                ) 
            {
                SimpleLog(simplelogger::simplemessage::MessageType::CRASH) << (*m_msg);
            }
        };

#elif __linux__
    STRUCT LIN_EXCEPTION : public SimpleException {

    }
#endif
    }

    /**
     * @brief Generating platform specific exceptions structs
     * 
     */
    namespace platformspec {
#ifdef _WIN32 || _WIN64
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_ACCESS_VIOLATION)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_ARRAY_BOUNDS_EXCEEDED)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_DATATYPE_MISALIGNMENT)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_FLT_DENORMAL_OPERAND)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_FLT_DIVIDE_BY_ZERO)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_FLT_INEXACT_RESULT)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_FLT_INVALID_OPERATION)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_FLT_OVERFLOW)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_FLT_STACK_CHECK)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_FLT_UNDERFLOW)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_ILLEGAL_INSTRUCTION)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_IN_PAGE_ERROR)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_INT_DIVIDE_BY_ZERO)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_INT_OVERFLOW)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_INVALID_DISPOSITION)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_NONCONTINUABLE_EXCEPTION)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_PRIV_INSTRUCTION)
        GENERATE_WIN_EXCEPTION_CLASS(EXCEPTION_STACK_OVERFLOW)
#elif __linux__

#endif
    }
}

#endif