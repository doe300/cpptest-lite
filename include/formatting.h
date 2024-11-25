#pragma once

#include <array>
#include <climits>
#include <cuchar>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#ifdef __has_include
#if __has_include(<span>)
#include <span>
#endif
#if __has_include(<optional>)
#include <optional>
#endif
#if __has_include(<variant>)
#include <variant>
#endif
#endif

namespace Test
{
	namespace Formats
	{

		////
		// Simple methods
		////

		inline std::string to_string(const char* val)
		{
			return std::string{"'"} + (val ? val : "(null)") + "'";
		}

		inline std::string to_string(const std::string& val)
		{
			return "'" + val + "'";
		}

#ifdef __cpp_char8_t
		inline std::string to_string(const std::u8string& val)
		{
			std::mbstate_t state{};
			std::string result;
			for(char8_t c : val)
			{
				std::array<char, MB_LEN_MAX> tmp{};
				std::size_t rc = std::c8rtomb(tmp.data(), c, &state);
				if (rc == static_cast<std::size_t>(-1))
				{
					result.push_back('?');
					state = std::mbstate_t{};
					rc = 0;
				}
				for (std::size_t i = 0; i < rc; ++i)
				{
					result.push_back(tmp[i]);
				}
			}
			return "'" + result + "'";
		}
#endif

		inline std::string to_string(const std::u16string& val)
		{
			std::mbstate_t state{};
			std::string result;
			for(char16_t c : val)
			{
				std::array<char, MB_LEN_MAX> tmp{};
				std::size_t rc = std::c16rtomb(tmp.data(), c, &state);
                if (rc == static_cast<std::size_t>(-1))
				{
					result.push_back('?');
					state = std::mbstate_t{};
					rc = 0;
				}
				for (std::size_t i = 0; i < rc; ++i)
				{
					result.push_back(tmp[i]);
				}
			}
			return "'" + result + "'";
		}

		inline std::string to_string(const std::u32string& val)
		{
			std::mbstate_t state{};
			std::string result;
			for(char32_t c : val)
			{
				std::array<char, MB_LEN_MAX> tmp{};
				std::size_t rc = std::c32rtomb(tmp.data(), c, &state);
                if (rc == static_cast<std::size_t>(-1))
                {
					result.push_back('?');
					state = std::mbstate_t{};
					rc = 0;
                }
				for (std::size_t i = 0; i < rc; ++i)
				{
					result.push_back(tmp[i]);
				}
			}
			return "'" + result + "'";
		}

#ifdef __cpp_lib_string_view
		inline std::string to_string(std::string_view val)
		{
			return "'" + std::string{val} + "'";
		}

		inline std::string to_string(std::u16string_view val)
		{
			std::mbstate_t state{};
			std::string result;
			for(char16_t c : val)
			{
				std::array<char, MB_LEN_MAX> tmp{};
				std::size_t rc = std::c16rtomb(tmp.data(), c, &state);
				if (rc == static_cast<std::size_t>(-1))
				{
					result.push_back('?');
					state = std::mbstate_t{};
					rc = 0;
				}
				for (std::size_t i = 0; i < rc; ++i)
				{
					result.push_back(tmp[i]);
				}
			}
			return "'" + result + "'";
		}

		inline std::string to_string(std::u32string_view val)
		{
			std::mbstate_t state{};
			std::string result;
			for(char32_t c : val)
			{
				std::array<char, MB_LEN_MAX> tmp{};
				std::size_t rc = std::c32rtomb(tmp.data(), c, &state);
				if (rc == static_cast<std::size_t>(-1))
				{
					result.push_back('?');
					state = std::mbstate_t{};
					rc = 0;
				}
				for (std::size_t i = 0; i < rc; ++i)
				{
					result.push_back(tmp[i]);
				}
			}
			return "'" + result + "'";
		}

#ifdef __cpp_char8_t
		inline std::string to_string(std::u8string_view val)
		{
			std::mbstate_t state{};
			std::string result;
			for(char8_t c : val)
			{
				std::array<char, MB_LEN_MAX> tmp{};
				std::size_t rc = std::c8rtomb(tmp.data(), c, &state);
				if (rc == static_cast<std::size_t>(-1))
				{
					result.push_back('?');
					state = std::mbstate_t{};
					rc = 0;
				}
				for (std::size_t i = 0; i < rc; ++i)
				{
					result.push_back(tmp[i]);
				}
			}
			return "'" + result + "'";
		}
#endif
#endif

#ifdef __cpp_char8_t
		inline std::string to_string(const char8_t* val)
		{
#ifdef __cpp_lib_string_view
			return to_string(std::u8string_view{val ? val : u8"(null)"});
#else
			return to_string(std::u8string{val ? val : u8"(null)"});
#endif
		}
#endif

		inline std::string to_string(const char16_t* val)
		{
#ifdef __cpp_lib_string_view
			return to_string(std::u16string_view{val ? val : u"(null)"});
#else
			return to_string(std::u16string{val ? val : u"(null)"});
#endif
		}

		inline std::string to_string(const char32_t* val)
		{
#ifdef __cpp_lib_string_view
			return to_string(std::u32string_view{val ? val : U"(null)"});
#else
			return to_string(std::u32string{val ? val : U"(null)"});
#endif
		}

		inline std::string to_string(std::nullptr_t)
		{
			return "(nullptr)";
		}

		inline std::string to_string(bool val)
		{
			return val ? "true" : "false";
		}

		inline std::string to_string(int32_t val)
		{
			return std::to_string(val);
		}

		inline std::string to_string(int64_t val)
		{
			return std::to_string(val);
		}

		inline std::string to_string(uint32_t val)
		{
			return std::to_string(val);
		}

		inline std::string to_string(uint64_t val)
		{
			return std::to_string(val);
		}

		////
		// Custom format
		////

		inline std::string to_string(float val)
		{
			char buffer[128];
			sprintf(buffer, "%g", static_cast<double>(val));
			return buffer;
		}

		inline std::string to_string(double val)
		{
			char buffer[128];
			sprintf(buffer, "%g", val);
			return buffer;
		}

		inline std::string to_string(long double val)
		{
			char buffer[128];
			sprintf(buffer, "%Lg", val);
			return buffer;
		}

		// support for enum-class, which can't be implicitly converted to int

		template<typename T>
		inline
		typename std::enable_if<std::is_enum<T>::value, std::string>::type
		to_string(const T val)
		{
			return std::to_string(static_cast<int>(val));
		}

		// support for generic pointers, which would otherwise be treated as booleans below
		template<typename T>
		inline
		typename std::enable_if<std::is_pointer<T>::value, std::string>::type
		to_string(const T ptr)
		{
			char buffer[32];
			sprintf(buffer, "%p", reinterpret_cast<const void*>(ptr));
			return buffer;
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

		////
		// User-defined functions
		////

		/*
		 * General to_string() for types, which define one of the following (in this order):
		 * - a stream-write operator (<<) with an std::ostream
		 * - a "to_string() const" member-function
		 * - a "toString() const" member-function
		 */

		template<typename T>
		inline
		typename std::enable_if<!std::is_enum<T>::value && !std::is_pointer<T>::value && internal::has_stream_operator<T>::value, std::string>::type
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

		namespace internal
		{
			template<std::size_t N, typename... Args>
			inline typename std::enable_if<N == (sizeof...(Args) - 1), std::string>::type toElementString(const std::tuple<Args...>& tuple)
			{
				return Test::Formats::to_string(std::get<N>(tuple));
			}

			template<std::size_t N, typename... Args>
			inline typename std::enable_if<N < (sizeof...(Args) - 1), std::string>::type toElementString(const std::tuple<Args...>& tuple)
			{
				return Test::Formats::to_string(std::get<N>(tuple)) + ", " + toElementString<N + 1>(tuple);
			}

			template<typename Head, typename Next, typename... Tail>
			inline std::string toElementString(const Head& head, const Next& next, const Tail&... tail)
			{
				return Test::Formats::to_string(head) + ", " + toElementString(next, tail...);
			}
		}

		template<typename... Args>
		inline std::string to_string(const std::tuple<Args...>& tuple)
		{
			return "{" + internal::toElementString<0>(tuple) + "}";
		}

		////
		// Container
		////
		// Need to be listed last to allow using the above user-defined to_string() definitions

		// support for optional values
#ifdef __cpp_lib_optional
		template<typename T>
		inline std::string to_string(const std::optional<T>& val)
		{
			if(val)
			{
				return to_string(*val);
			}
			return "(empty)";
		}
#endif

#ifdef __cpp_lib_variant
		template<typename... Args>
		inline std::string to_string(const std::variant<Args...>& val)
		{
			return std::visit([] (auto& entry) {return to_string(entry);}, val);
		}
#endif

		// support for read-only memory ranges (e.g. vector, array, etc.)
		template<typename T>
		inline std::string to_string(T begin, T end)
		{
			std::stringstream ss;
			ss << '(' << std::distance(begin, end) << ") [";
			bool first = true;
			for(auto it = begin; it != end; ++it)
			{
				if(!first) ss << ", ";
				first = false;
				ss << to_string(*it);
			}
			ss << ']';
			return ss.str();
		}

		template<typename T, std::size_t N>
		inline std::string to_string(const std::array<T, N>& val)
		{
			return to_string(val.begin(), val.end());
		}

		template<typename T>
		inline std::string to_string(const std::vector<T>& val)
		{
			return to_string(val.begin(), val.end());
		}

#ifdef __cpp_lib_span
		template<typename T>
		inline std::string to_string(const std::span<T>& val)
		{
			return to_string(val.begin(), val.end());
		}
#endif
	} // namespace Formats
} // namespace Test
