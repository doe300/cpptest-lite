#pragma once

#include <algorithm>
#include <cmath>
#include <string>
#include <type_traits>
#ifdef __has_include
#if __has_include(<span>)
#include <span>
#endif
#endif

namespace Test
{
	namespace Comparisons
	{
		template<typename T>
		struct Predicate
		{
			bool operator()(const T& obj) const;
		};

		template<typename T>
		struct BiPredicate
		{
			bool operator()(const T& obj1, const T& obj2) const;
		};

		template<typename T>
		constexpr bool inMaxDistance(const T& source, const T& compare, const T& maxDistance)
		{
			static_assert(std::is_arithmetic<T>(), "Can only compare arithmetic types");
			return ((compare - source) * ( compare > source ? 1 : -1)) <= (maxDistance < 0 ? -maxDistance : maxDistance);
		}

		template<typename T>
		constexpr bool inRange(const T& lowerLimit, const T& upperLimit, const T& value)
		{
			static_assert(std::is_arithmetic<T>(), "Can only compare arithmetic types");
			return lowerLimit < upperLimit ? (lowerLimit <= value && value <= upperLimit) : (upperLimit <= value && value <= lowerLimit);
		}

		template<typename T>
		inline bool inRangeObject(const T& lowerLimit, const T& upperLimit, const T& value)
		{
			return (lowerLimit < upperLimit) ? (lowerLimit <= value && value <= upperLimit) : (upperLimit <= value && value <= lowerLimit);
		}

		////
		// Special methods
		////

		inline bool isSame(const double val1, const double val2)
		{
			if(std::isnan(val1) && std::isnan(val2))
				return true;
			return val1 == val2;
		}

#ifdef __cpp_lib_span
		template<typename T>
		inline bool isSame(const std::span<T>& span1, const std::span<T>& span2)
		{
			if(span1.size() != span2.size())
				return false;
			return std::equal(span1.begin(), span1.end(), span2.begin(), span2.end());
		}
#endif

		////
		// General case
		////

		//support for arbitrary types, falls back to the == operator
		template<typename T1, typename T2>
		inline bool isSame(const T1& val1, const T2& val2)
		{
			return val1 == val2;
		}
	} // namespace Comparisons
} // namespace Test
