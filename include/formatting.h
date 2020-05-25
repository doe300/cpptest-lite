/* 
 * File:   formatting.h
 * Author: doe300
 *
 * Created on March 4, 2017, 11:47 AM
 */

#ifndef FORMATTING_H
#define FORMATTING_H

#include <iostream>
#include <sstream>
#include <string>

namespace Test
{
	namespace Formats
	{

		////
		// Simple methods
		////

		inline std::string to_string(const char* val)
		{
			return val;
		}

		inline std::string to_string(const std::string& val)
		{
			return val;
		}

		inline std::string to_string(const std::nullptr_t ptr)
		{
			return "(nullptr)";
		}

		inline std::string to_string(const int32_t val)
		{
			return std::to_string(val);
		}

		inline std::string to_string(const int64_t val)
		{
			return std::to_string(val);
		}

		inline std::string to_string(const uint32_t val)
		{
			return std::to_string(val);
		}

		inline std::string to_string(const uint64_t val)
		{
			return std::to_string(val);
		}

		////
		// Custom format
		////

		inline std::string to_string(const float val)
		{
			char buffer[128];
			sprintf(buffer, "%g", static_cast<double>(val));
			return buffer;
		}

		inline std::string to_string(const double val)
		{
			char buffer[128];
			sprintf(buffer, "%g", val);
			return buffer;
		}

		inline std::string to_string(const long double val)
		{
			char buffer[128];
			sprintf(buffer, "%Lg", val);
			return buffer;
		}

		//support for enum-class, which can't be implicitly converted to int

		template<typename T>
		inline
		typename std::enable_if<std::is_enum<T>::value, std::string>::type
		to_string(const T val)
		{
			return std::to_string(static_cast<int>(val));
		}

		/*
		 * Helper for checking whether #to_string() or #toString() member-functions exist
		 *
		 * Based on the example in section 28.4.4 of "The C++ Programming Language", 4th edition, Stroustrup, 2013
		 */
		namespace internal
		{
			struct substitution_failure { };

			template<typename T>
			struct substitution_succeeded : std::true_type { };

			template<>
			struct substitution_succeeded<substitution_failure> : std::false_type { };

			template<typename T>
			struct get_to_string_result
			{
			private:
				template<typename X>
				static auto check(X const& x) -> decltype(x.to_string());
				static substitution_failure check(...);
			public:
				using type = decltype(check(std::declval<T>()));
			};

			template<typename T>
			struct get_toString_result
			{
			private:
				template<typename X>
				static auto check(X const& x) -> decltype(x.toString());
				static substitution_failure check(...);
			public:
				using type = decltype(check(std::declval<T>()));
			};

			template<typename T>
			struct get_stream_operator_result
			{
			private:
				template<typename X>
				static auto check(X const& x) -> decltype(std::declval<std::ostream&>() << std::declval<X>());
				static substitution_failure check(...);
			public:
				using type = decltype(check(std::declval<T>()));
			};


			template<typename T>
			struct has_to_string : substitution_succeeded<typename get_to_string_result<T>::type> { };

			template<typename T>
			struct has_toString : substitution_succeeded<typename get_toString_result<T>::type> { };

			template<typename T>
			struct has_stream_operator : substitution_succeeded<typename get_stream_operator_result<T>::type> { };
		} // namespace internal

		/*
		 * General to_string() for types, which define one of the following (in this order):
		 * - a stream-write operator (<<) with an std::ostream
		 * - a "to_string() const" member-function
		 * - a "toString() const" member-function
		 */

		template<typename T>
		inline
		typename std::enable_if<!std::is_enum<T>::value && internal::has_stream_operator<T>::value, std::string>::type
		to_string(const T& val)
		{
			std::stringstream s;
			s << val;
			return s.str();
		}

		template<typename T>
		typename std::enable_if<internal::has_to_string<T>::value && !internal::has_stream_operator<T>::value, std::string>::type
		to_string(const T& val)
		{
			return val.to_string();
		}

		template<typename T>
		typename std::enable_if<internal::has_toString<T>::value && !internal::has_to_string<T>::value && !internal::has_stream_operator<T>::value, std::string>::type
		to_string(const T& val)
		{
			return val.toString();
		}
	} // namespace Formats
} // namespace Test

#endif /* FORMATTING_H */

