/* 
 * File:   ConsoleOutput.h
 * Author: doe300
 *
 * Created on April 25, 2016, 4:43 PM
 */

#ifndef CONSOLEOUTPUT_H
#define CONSOLEOUTPUT_H

#include "Output.h"
#include "TextOutput.h"

namespace Test
{

	class ConsoleOutput : public TextOutput
	{
	public:
		explicit ConsoleOutput(unsigned int mode);
		ConsoleOutput(const ConsoleOutput&) = delete;
		ConsoleOutput(ConsoleOutput&&) noexcept = delete;
		~ConsoleOutput() override = default;

		ConsoleOutput& operator=(const ConsoleOutput&) = delete;
		ConsoleOutput& operator=(ConsoleOutput&&) noexcept = delete;

		void finishSuite(const std::string& suiteName, unsigned int numTests, unsigned int numPositiveTests, std::chrono::microseconds totalDuration) override;

		void finishTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString, bool withSuccess) override;

		void printSuccess(const Assertion& assertion) override;
		void printFailure(const Assertion& assertion) override;
		void printException(const std::string& suiteName, const std::string& methodName, const std::string& argString, const std::exception& ex) override;
	private:
		static const std::string errorColor;
		static const std::string successColor;
		static const std::string resetColors;
	};

} // namespace Test
#endif /* CONSOLEOUTPUT_H */

