#ifndef SIMPLE_LOGGER_FACTORY
#define SIMPLE_LOGGER_FACTORY

#include <functional>

namespace simplelogger::settings {
    class Settings;
}

namespace simplelogger::utils {

    /**
     * @brief 
     * 
     * @tparam T 
     */
    template <typename T>
    class Factory {
    public:

        /**
         * @brief Construct a new Factory object
         * 
         */
        Factory() = default;

        /**
         * @brief Construct a new Factory object
         * 
         */
        Factory(Factory&&) = default;

        /**
         * @brief Destroy the Factory object
         * 
         */
        ~Factory() {}

        /**
         * @brief Construct a new Factory object
         * 
         */
        Factory(Factory&) = delete;

        /**
         * @brief 
         * 
         * @return Factory& 
         */
        Factory& operator=(Factory&) = delete;

        /**
         * @brief 
         * 
         * @return Factory& 
         */
        Factory& operator=(Factory&&) = delete;

        /**
         * @brief Set the option object
         * 
         * @tparam Opt 
         * @tparam Val 
         * @param opt 
         * @param val 
         */
        template <typename Opt, typename Val>
        Factory<T>& set_option(Opt opt, Val&& val) {
            m_object_ptr->set_option(opt, std::forward<Val>(val));
            return *this;
        }

        std::unique_ptr<T> create() {
            return std::move(m_object_ptr);
        }

    private:
        std::unique_ptr<T> m_object_ptr;
    };
}

#endif