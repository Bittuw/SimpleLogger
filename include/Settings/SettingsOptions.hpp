#ifndef SIMPLE_LOGGER_SETTINGS_OPTIONS
#define SIMPLE_LOGGER_SETTINGS_OPTIONS

#include <string>

namespace simplelogger::settings::options {

    namespace objecttypes {

        /**
         * @brief 
         * 
         */
        enum class SinkTypes {

            /**
             * @brief File stream
             * 
             */
            TO_FILE = 0x00,

            /**
             * @brief TCP sockets stream
             * 
             */
            TO_TCP = 0x01, 

            /**
             * @brief UDP sockets stream
             * 
             */
            TO_UDP = 0x02,

            /**
             * @brief Programm output stream
             * 
             */
            TO_STD = 0x04,

            /**
             * @brief Using stream, 
             * that will inserted, of not, then not work
             */
            TO_STREAM = 0x08,

            /**
             * @brief Future, now default TO_STD, for user implementation
             * 
             */
            TO_CUSTOM = 0x10
        };

        /**
         * @brief 
         * 
         */
        enum class FilterTypes {/* Empty yet!*/};

        /**
         * @brief 
         * 
         */
        enum class FormatterTypes {/* Empty yet!*/};
    }

#define GENERATE_OPTIONS(OPTION, INTERNAL_TYPE)\
struct option_##OPTION{\
    option_##OPTION(INTERNAL_TYPE& value) : m_value(value){}\
    option_##OPTION(INTERNAL_TYPE&& value) : m_value(std::move(value)){}\
    INTERNAL_TYPE& get_value() { return m_value; }\
    private:\
    INTERNAL_TYPE m_value;\
};

    /**
     * @brief 
     * 
     */
    namespace loggeroption {

        /**
         * @brief Using for main Logger / Future
         * 
         */
        enum class Options {
            #define X(TYPE, NAME) NAME
                SIMPLE_LOGGER_OPTIONS_X_COMMA
            #undef X
        };

        /**
         * @brief 
         * 
         */
        // #define X(TYPE, NAME) GENERATE_OPTIONS(NAME, TYPE)
        //     SIMPLE_LOGGER_OPTIONS_X    
        // #undef X
    }

    /**
     * @brief For internal objects
     * 
     */
    namespace objectoptions {

        /**
         * @brief 
         * 
         */
        enum class SinkOptions {

            /**
             * @brief Internal name to resolver
             * 
             */
            NAME = 0x00,

            /**
             * @brief Type, SinkTypes
             * 
             */
            TYPE = 0x01,

            /**
             * @brief Rotate time
             * 
             */
            ROTATE = 0x02,

            /**
             * @brief Buffer size
             * 
             */
            BUFFER = 0x04,

            /**
             * @brief Filter
             * 
             */
            FILTER = 0x08,
        };

        /**
         * @brief 
         * 
         */
        enum class FilterOptions {

            /**
             * @brief Internal name to resolve
             * 
             */
            NAME = 0x00,

            /**
             * @brief Message Mask (DEBUG || ERROR)
             * 
             */
            MASK = 0x01,

            /**
             * @brief Custom Attributes / Future
             * 
             */
            ATTRIBUTES = 0x02,

            /**
             * @brief Internal formatter
             * 
             */
            FORMATTER = 0x04
        };

        /**
         * @brief Using Formatters
         * 
         */
        enum class FormatterOptions {
            
            /**
             * @brief Internal name to resolve
             * 
             */
            NAME = 0x00,

            /**
             * @brief String like format ()
             * 
             */
            FORMATE_STRING = 0x01
        };

        // enum class AttributesOptions {

        //     // /**
        //     //  * @brief Internal name to resolve
        //     //  * 
        //     //  */
        //     // NAME = 0x00,

        //     // /**
        //     //  * @brief 
        //     //  * 
        //     //  */
        //     // TYPE = 0x01,

        //     // /**
        //     //  * @brief 
        //     //  * 
        //     //  */

        // };
    }
}

#endif