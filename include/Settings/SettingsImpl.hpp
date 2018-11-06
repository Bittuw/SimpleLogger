#ifndef SIMPLE_LOGGER_SETTINGS_IMPL
#define SIMPLE_LOGGER_SETTINGS_IMPL

#include <map>

#include "Settings/SettingsOptions.hpp"

/**
 * @brief 
 * 
 */
namespace simplelogger::settings {

    /**
     * @brief 
     * 
     */
    class SettingsImpl {
    public:
        /**
         * @brief Push settings to logger
         * 
         */
        void push_settings();
    private:

        /**
         * @brief 
         * 
         */
        std::map<options::objectoptions::SinkOptions, std::string> m_options;
    };
}

#endif