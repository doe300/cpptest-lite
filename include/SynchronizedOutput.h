#pragma once

#include "Output.h"

#include <mutex>

namespace Test
{

	/*!
	 * An Output which synchronizes all access to the underlying output making it safe for usage in a multi-threaded environment
	 */
	class SynchronizedOutput : public Output
	{
	public:
		explicit SynchronizedOutput(Output& realOutput);
		SynchronizedOutput(const SynchronizedOutput&) = delete;
		SynchronizedOutput(SynchronizedOutput&&) noexcept = delete;
		~SynchronizedOutput() override = default;

		SynchronizedOutput& operator=(const SynchronizedOutput&) = delete;
		SynchronizedOutput& operator=(SynchronizedOutput&&) noexcept = delete;

		void initializeSuite(const std::string& suiteName, unsigned int numTests) override;
		void finishSuite(const std::string& suiteName, unsigned int numTests, unsigned int numPositiveTests, std::chrono::microseconds totalDuration) override;
		void initializeTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString) override;
		void finishTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString, bool withSuccess) override;

		void printException(const std::string& suiteName, const std::string& methodName, const std::string& argString, const std::exception& ex) override;
		void printSuccess(const Assertion& assertion) override;
		void printFailure(const Assertion& assertion) override;
	private:
		Output& realOutput;
		std::mutex outputMutex;
	};
} // namespace Test
