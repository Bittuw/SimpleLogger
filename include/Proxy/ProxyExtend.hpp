#ifndef SIMPLE_PROXY_EXTEND
#define SIMPLE_PROXY_EXTEND

#include <type_traits>
#include <functional>

#include "Proxy/Proxy.hpp"
#include "Proxy/ProxyImplInterface.hpp"

namespace simplelogger::simpleproxy {

	///
	/// Cleaning from cv
	///
	template<typename Type>
	using Cleaning = std::remove_const_t<std::remove_reference_t<Type>>;

	///
	/// Base
	///
	template <typename Base, typename Der>
	using is_base = std::is_base_of<Base, Cleaning<Der>>;

	///
	/// For fundamental
	///
	template <typename Arg, 
		typename std::enable_if_t<std::is_integral_v<Cleaning<Arg>>, Cleaning<Arg>>* = nullptr>
	inline Proxy&& operator<<(Proxy&& proxy, Arg&& t) {
		return operator<<(std::move(proxy), std::to_string(std::move(t)));
	}

	template <typename Arg,
		typename std::enable_if_t<std::is_integral_v<Cleaning<Arg>>, Cleaning<Arg>>* = nullptr>
	inline Proxy&& operator<<(Proxy&& proxy, Arg& t) {
		return operator<<(std::move(proxy), std::to_string(t));
	}
	///
	/// For ToStringCRTP based
	///
	template <typename Arg,
		typename std::enable_if_t<std::is_base_of_v<ToStringCRTP<Cleaning<Arg>>, Cleaning<Arg>>, Cleaning<Arg>>* = nullptr>
	inline Proxy&& operator<< (Proxy&& proxy, ToStringCRTP<Arg>&& t) {
		auto aut = std::move(t);
		return operator<<(std::move(proxy), aut.string_presenter());
	}

	template <typename Arg,
		typename std::enable_if_t<std::is_base_of_v<ToStringCRTP<Cleaning<Arg>>, Cleaning<Arg>>, Cleaning<Arg>>* = nullptr>
	inline Proxy&& operator<< (Proxy&& proxy, ToStringCRTP<Arg>& t) {
		return operator<<(std::move(proxy), t.string_presenter());
	}
	///
	/// For functions, labdas and raw functors
	///
	inline Proxy&& operator<< (Proxy&& proxy, std::function<std::string()> function) {
		return operator<<(std::move(proxy), function());
	}

}

#endif