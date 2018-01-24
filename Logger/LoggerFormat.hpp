#pragma once

#ifndef LOGGER_FORMATE_HPP
#define LOGGER_FORMATE_HPP

#ifndef __cplusplus
#error This header can be only used with C++ code
#endif

#include <iostream>
#include <sstream>
#include <tuple>

namespace LoggerFormat {
	template<typename Tuple, std::size_t N>
	struct TuplePrinter {
		static void print(const std::string& fmt, std::ostream& os, const Tuple& t) {
			const size_t idx = fmt.find_last_of('%');
			TuplePrinter<Tuple, N - 1>::print(std::string(fmt, 0, idx), os, t);
			os << std::get<N - 1>(t) << std::string(fmt, idx + 1);
		}
	};

	template<typename Tuple>
	struct TuplePrinter<Tuple, 1> {
		static void print(const std::string& fmt, std::ostream& os, const Tuple& t) {
			const size_t idx = fmt.find_first_of('%');
			os << std::string(fmt, 0, idx) << std::get<0>(t) << std::string(fmt, idx + 1);
		}
	};

	template<typename... Args>
	std::string format(const std::string& fmt, Args&&... args) {
		std::stringstream ss;
		const auto t = std::make_tuple(std::forward<Args>(args)...);
		TuplePrinter<decltype(t), sizeof...(Args)>::print(fmt, ss, t);
		return ss.str();
	}
}
#endif