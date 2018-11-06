#ifndef SIMPLE_LOGGER_SETTINGS
#define SIMPLE_LOGGER_SETTINGS

#include <memory>

#include "Settings/SettingsOptions.hpp"
#include "Utils/Factory.hpp"

/**
 * @brief 
 * 
 */
namespace simplelogger::settings {

    class SettingsImpl;

    /**
     * @brief 
     * 
     */
    class Settings {
    public:

        /**
         * @brief Create a filter object (Create Factory<Filter>())
         * 
         */
        utils::Factory<Filter> create_filter();

        /**
         * @brief Create a sink object (Create Factory<Sink>())
         * 
         */
        utils::Factory<Sink> create_sink();

        /**
         * @brief Create a formatter object (Create Factory<Formatter>())
         * 
         */
        utils::Factory<Formatter> create_formatter();

    private:
        std::unique_ptr<SettingsImpl> m_impl;
    };
}

#endif