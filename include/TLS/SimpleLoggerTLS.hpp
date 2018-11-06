#ifndef SIMPLE_LOGGER_TLS
#define SIMPLE_LOGGER_TLS

#include <memory>

/**
 * @brief 
 * 
 */
namespace simplelogger::tls {

    class SimpleLoggerTLSImpl;

    /**
     * @brief Class, that present thread local instance of SimpleLogger,
     * all communication with logger through this instance
     * 
     */
    class SimpleLoggerTLS {
    public:

        /**
         * @brief Get the thread local instance object
         * 
         * @return SimpleLoggerTLS& 
         */
		static SimpleLoggerTLS& get_thread_local_instance() noexcept;
		~SimpleLoggerTLS() noexcept;

        /**
         * @brief
         * 
         */
        SimpleLoggerTLS(SimpleLoggerTLS&) = delete;

        /**
         * @brief Construct a new Simple Logger T L S object
         * 
         */
        SimpleLoggerTLS(SimpleLoggerTLS&&) = delete;

        /**
         * @brief 
         * 
         * @return SimpleLoggerTLS& 
         */
        SimpleLoggerTLS& operator=(SimpleLoggerTLS&) = delete;

        /**
         * @brief 
         * 
         * @return SimpleLoggerTLS& 
         */
        SimpleLoggerTLS& operator=(SimpleLoggerTLS&&) = delete;

    private:
        
        /**
         * @brief Construct a new Simple Logger T L S object
         * 
         */
        SimpleLoggerTLS() noexcept;

        /**
         * @brief 
         * 
         */
        std::unique_ptr<SimpleLoggerTLSImpl> m_impl;
    };
}

#endif SIMPLE_LOGGER_TLS