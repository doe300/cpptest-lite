/* 
 * File:   formatting.h
 * Author: doe300
 *
 * Created on March 4, 2017, 11:47 AM
 */

#ifndef FORMATTING_H
#define FORMATTING_H

#include <string>
#include <iostream>
#include <sstream>

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

        inline std::string to_string(const std::string val)
        {
            return val;
        }

        inline std::string to_string(const std::nullptr_t ptr)
        {
            return "(nullptr)";
        }

        inline std::string to_string(const int val)
        {
            return std::to_string(val);
        }

        inline std::string to_string(const long val)
        {
            return std::to_string(val);
        }

        inline std::string to_string(const long long val)
        {
            return std::to_string(val);
        }

        inline std::string to_string(const unsigned val)
        {
            return std::to_string(val);
        }

        inline std::string to_string(const unsigned long val)
        {
            return std::to_string(val);
        }

        inline std::string to_string(const unsigned long long val)
        {
            return std::to_string(val);
        }

        ////
        // Custom format
        ////

        inline std::string to_string(const float val)
        {
            char buffer[128];
            sprintf(buffer, "%g", val);
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
            return std::to_string((int) val);
        }

        //support for arbitrary types that support the << operator (excluding enums, which are handled above)
        template<typename T>
        inline
		typename std::enable_if<!std::is_enum<T>::value, std::string>::type
		to_string(const T& val)
        {
        	std::stringstream s;
        	s << val;
        	return s.str();
        }
    }
}

#endif /* FORMATTING_H */

