#ifndef SIMPLE_OPERATORS
#define SIMPLE_OPERATORS

#include <type_traits>
#include <iostream>

namespace simplelogger::simpleoperators {
	template <typename D>
	struct Operators {
		constexpr friend bool operator>(const D& left, const D& right) {
			return !static_cast<bool>(left < right) && static_cast<bool>(right < left);
		}
		constexpr friend bool operator==(const D& left, const D& right) {
			return !static_cast<bool>(left < right) && !static_cast<bool>(right < left);
		}
		constexpr friend bool operator<=(const D& left, const D& right) {
			return !static_cast<bool>(right < left);
		}
		constexpr friend bool operator>=(const D& left, const D& right) {
			return !static_cast<bool>(left < right);
		}
		friend bool operator!=(const D& left, const D& right) {
			return !(static_cast<bool>(left < right) || static_cast<bool>(right < left));
		}
	};
}

#endif