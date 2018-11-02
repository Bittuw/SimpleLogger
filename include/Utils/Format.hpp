#ifndef SIMPLE_LOGGER_UTILS
#define SIMPLE_LOGGER_UTILS

#include <sstream>
#include <string>


namespace simplelogger::utils {
template<typename Tuple, std::size_t N, typename StringType, typename StreamType>
	struct TuplePrinter {
		static void print(const StringType& fmt, StreamType& os, const Tuple& t) {
			const size_t idx = static_cast<std::size_t>(fmt.find_last_of(StringType::value_type('%')));
			TuplePrinter<Tuple, N - 1, StringType, StreamType>::print(StringType(fmt, 0, idx), os, t);
			os << std::get<N - 1>(t) << StringType(fmt, idx + 1);
		}
	};

	template<typename Tuple, typename StringType, typename StreamType>
	struct TuplePrinter<Tuple, 1, StringType, StreamType> {
		static void print(const StringType& fmt, StreamType& os, const Tuple& t) {
			const size_t idx = static_cast<std::size_t>(fmt.find_last_of(StringType::value_type('%')));
			os << StringType(fmt, 0, idx) << std::get<0>(t) 
				<< StringType(fmt, idx + 1);
		}
	};

	template<typename... Args>
	inline std::string format(const std::string& fmt, Args&&... args) {
		std::stringstream ss;
		const auto t = std::make_tuple(std::forward<Args>(args)...);
		TuplePrinter<decltype(t), sizeof...(Args), std::string, std::ostream>
			::print(fmt, ss, t);
		return ss.str();
	}

    template<typename... Args>
	inline std::wstring format(const std::wstring& fmt, Args&&... args) {
		std::wstringstream ss;
		const auto t = std::make_tuple(std::forward<Args>(args)...);
		TuplePrinter<decltype(t), sizeof...(Args), std::wstring, std::wostream>
			::print(fmt, ss, t);
		return ss.str();
	}
}
#endif