#ifndef SIMPLE_SETTINGS_PIMPL 
#define SIMPLE_SETTINGS_PIMPL

#include <memory>

/**
 * @brief 
 * 
 */
namespace simplelogger::utils {

    /**
     * @brief CRTP helper
     * 
     * @tparam Derived
     */
    template <typename Derived>
    struct CRTP {
        T& cast() { return static_cast<T&>(*this);}
        T const& cast() { return static_cast<T const&>(*this)}
    // private:
    //     CRTP() = default;
    //     friend CRTP_Derived<Derived>;
    };
}

#endif