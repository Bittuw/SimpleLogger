#ifndef SIMPLE_LOGGER_TLS_IMPL
#define SIMPLE_LOGGER_TLS_IMPL

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
#elif __linux__
#endif
    };
}

#endif