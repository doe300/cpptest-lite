#pragma once

#include "Output.h"

namespace Test
{

	class TextOutput : public Output
	{
	public:

		static const unsigned int Debug = 1;
		static const unsigned int Verbose = 2;
		static const unsigned int Terse = 3;

		explicit TextOutput(unsigned int mode);
		TextOutput(unsigned int mode, std::ostream& stream);
		TextOutput(const TextOutput&) = delete;
		TextOutput(TextOutput&&) noexcept = delete;
		~TextOutput() override;

		TextOutput& operator=(const TextOutput&) = delete;
		TextOutput& operator=(TextOutput&&) noexcept = delete;

		void initializeSuite(const std::string& suiteName, unsigned int numTests) override;
		void finishSuite(const std::string& suiteName, unsigned int numTests, unsigned int numPositiveTests, std::chrono::microseconds totalDuration) override;

		void initializeTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString) override;
		void finishTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString, bool withSuccess) override;


		void printSuccess(const Assertion& assertion) override;
		void printFailure(const Assertion& assertion) override;
		void printException(const std::string& suiteName, const std::string& methodName, const std::string& argString, const std::exception& ex) override;

	protected:
		std::ostream& stream;
		const unsigned int mode;
	};

} // namespace Test
