#pragma once

#include <chrono>
#include <exception>
#include <string>

namespace Test
{
	namespace Private
	{

		/*!
		 * Returns the file-name without any folders extracted from the given file-path
		 */
		inline std::string getFileName(const std::string& file)
		{
			std::string fileName = file;
			std::size_t pos = fileName.find_last_of('\\');
			if (pos != std::string::npos)
				fileName = fileName.substr(pos + 1);
			pos = fileName.find_last_of('/');
			if (pos != std::string::npos)
				fileName = fileName.substr(pos + 1);
			return fileName;
		}
	} // namespace Private

	struct Assertion
	{
		std::string suite;
		const std::string file;
		std::string method;
		std::string args;
		std::string errorMessage;
		const std::string userMessage;
		const int lineNumber;

		Assertion(const char* fileName, const int lineNumber, const std::string& errorMessage, const char* userMessage) :
			suite(""), file(fileName), method(""), args(""), errorMessage(errorMessage), userMessage(userMessage), lineNumber(lineNumber)
		{
		}

		Assertion(const char* fileName, const int lineNumber, const char* userMessage) :
			suite(""), file(fileName), method(""), args(""), errorMessage(""), userMessage(userMessage), lineNumber(lineNumber)
		{
		}

		Assertion(const char* fileName, const int lineNumber) :
			suite(""), file(fileName), method(""), args(""), errorMessage(""), userMessage(""), lineNumber(lineNumber)
		{
		}
	};

	/*!
	 * Base class for all kinds of Outputs
	 */
	class Output
	{
	public:
		Output(const Output& orig) = delete;
		Output(Output&&) noexcept = default;
		virtual ~Output() noexcept = default;

		Output& operator=(const Output&) = delete;
		Output& operator=(Output&&) noexcept = default;

		/*!
		 * Initializes the output for a single suite
		 *
		 * \param suiteName The name of the suite
		 * \param numTests The number of tests in this suite
		 */
		virtual void initializeSuite(const std::string& suiteName, const unsigned int numTests)
		{
		}

		/*!
		 * Finishes the output for a single suite
		 *
		 * \param suiteName The name of the suite
		 * \param numTests The number of tests in the suite
		 * \param numPositiveTests The number of successful tests in this suite
		 * \param totalDuration The total duration in microseconds the suite took to execute
		 */
		virtual void finishSuite(const std::string& suiteName, const unsigned int numTests, const unsigned int numPositiveTests, const std::chrono::microseconds totalDuration)
		{
		}

		/*!
		 * Initializes the output for a single test-method
		 *
		 * \param suiteName The name of the suite
		 * \param methodName The name of the test-method
		 * \param argString The string-representation of the argument
		 */
		virtual void initializeTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString)
		{
		}

		/*!
		 * Finished the output for a single test-method
		 *
		 * \param suiteName The name of the suite
		 * \param methodName The name of the test-method
		 * \param argString The argument-string for the test-method
		 * \param withSuccess Whether the test-method was finished with success
		 */
		virtual void finishTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString, const bool withSuccess)
		{
		}

		/*!
		 * Finished the output for a single test-method that threw an error
		 *
		 * NOTE: when this method is called, \ref finishTestMethod is skipped for this test-method
		 *
		 * \param suiteName The name of the suite
		 * \param methodName The name of the test-method
		 * \param argString The argument-string for the test-method
		 * \param ex The exception, that was thrown
		 */
		virtual void printException(const std::string& suiteName, const std::string& methodName, const std::string& argString, const std::exception& ex)
		{
		}

		/*!
		 * Prints a successful test
		 *
		 * NOTE: this method is only called, if the macro CPPTEST_LITE_LOG_SUCCESS is set
		 *
		 * \param assertion Information about the successful assertion
		 */
		virtual void printSuccess(const Assertion& assertion)
		{
		}

		/*!
		 * Prints a failed test
		 *
		 * \param assertion Information about the failed assertion
		 */
		virtual void printFailure(const Assertion& assertion)
		{
		}

	protected:

		Output() = default;

		/*!
		 * Converts a fraction into a percentage-value with 2 decimals
		 */
		inline double prettifyPercentage(const double part, const double whole) const
		{
			if(part == 0 || whole == 0)
			{
				return 0;
			}
			int tmp = static_cast<int>((part / whole) * 10000);
			return tmp / 100.0;
		}

		/*!
		 * Removes the class-name from the given methodName
		 */
		inline std::string stripMethodName(const std::string& methodName)
		{
			std::size_t pos = methodName.size() - 1;
			while(pos > 0)
			{
				if(methodName[pos] == ':')
					return methodName.substr(pos+1);
				--pos;
			}
			return methodName;
		}

		/*!
		 * Truncates the string to the given length and adding trailing "..." if necessary
		 */
		inline std::string truncateString(const std::string& string, unsigned int length)
		{
			if(string.size() <= length)
			{
				return string;
			}
			return string.substr(0, length-3) + "...";
		}
	};

} // namespace Test
