#ifndef SIMPLE_PROXY_INMPL_INTERFACE
#define SIMPLE_PROXY_INMPL_INTERFACE

#include <utility>

namespace simplelogger::simpleproxy {

	/**
	 * @brief 
	 * 
	 */
    struct StringInvoke {

		/**
		 * @brief 
		 * 
		 * @tparam Impl 
		 * @tparam Args 
		 * @param impl 
		 * @param args 
		 * @return std::string 
		 */
		template <typename Impl, typename ...Args>
		static std::string to_string(Impl* impl, Args&&...args) {
			return impl->to_string(std::forward<Args>(args)...);
		}
	};

	/**
	 * @brief 
	 * 
	 * @tparam D 
	 */
	template <typename D>
	struct ToStringCRTP {

		/**
		 * @brief 
		 * 
		 * @tparam Args 
		 * @return std::string 
		 */
		template <typename ...Args>
		std::string string_presenter() {
			return StringInvoke::to_string(reinterpret_cast<D*>(this));
		}
	};

}

#endif