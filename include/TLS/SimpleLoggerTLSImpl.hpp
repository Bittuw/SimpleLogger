#ifndef SIMPLE_LOGGER_TLS_IMPL
#define SIMPLE_LOGGER_TLS_IMPL

#ifdef _WIN32 || _WIN64
    #include "Utils/SehGuard.hpp"
#elif __linux__
#endif

// NOTE: if windows, we only use _set_se_translator for translate 

/**
 * @brief 
 * 
 */
namespace simplelogger::tls {

    /**
     * @brief 
     * 
     */
    class SimpleLoggerTLSImpl {
    public:

        /**
         * @brief Construct a new Simple Logger T L S Impl object
         * 
         */
        SimpleLoggerTLSImpl() noexcept;

        /**
         * @brief Destroy the Simple Logger T L S Impl object
         * 
         */
        ~SimpleLoggerTLSImpl() noexcept;

        /**
         * @brief 
         * 
         */
        void init_failure_capture() noexcept; // For WIN32 AND Linux impl;

#ifdef _WIN32
        /**
         * @brief Using fot catch all win exception
         * 
         */
        simplelogger::utils::SehGuard auto_guard; 
#elif __linux__
#endif
    };
}

#endif