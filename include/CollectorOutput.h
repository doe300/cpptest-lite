/* 
 * File:   CollectorOutput.h
 * Author: daniel
 *
 * Created on September 20, 2015, 12:16 PM
 */

#ifndef COLLECTOROUTPUT_H
#define	COLLECTOROUTPUT_H

#include "Output.h"

#include <vector>

namespace Test
{

	/*!
	 * A collector-output collects all information to be processed after all tests have run
	 */
	class CollectorOutput : public Output
	{
	public:
		CollectorOutput() = default;
		CollectorOutput(const CollectorOutput&) = delete;
		CollectorOutput(CollectorOutput&&) = default; //RPi cross-compiler throws on noexcept here
		~CollectorOutput() noexcept override = default;

		CollectorOutput& operator=(const CollectorOutput&) = delete;
		CollectorOutput& operator=(CollectorOutput&&) = default; //RPi cross-compiler throws on noexcept here

		void initializeSuite(const std::string& suiteName, unsigned int numTests) override;
		void finishSuite(const std::string& suiteName, unsigned int numTests, unsigned int numPositiveTests, std::chrono::microseconds totalDuration) override;

		void initializeTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString) override;
		void finishTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString, bool withSuccess) override;

		void printException(const std::string& suiteName, const std::string& methodName, const std::string& argString, const std::exception& ex) override;
		void printSuccess(const Assertion& assertion) override;
		void printFailure(const Assertion& assertion) override;

	protected:

		struct TestMethodInfo
		{
			const std::string methodName;
			const std::string argString;
			std::vector<Assertion> failedAssertions;
			std::vector<Assertion> passedAssertions;
			std::string exceptionMessage;
			bool withSuccess;

			TestMethodInfo(const std::string& name, const std::string& argString) : methodName(name), argString(argString),
					failedAssertions({}), passedAssertions({}), exceptionMessage(""), withSuccess(false)
			{ }
		};

		struct SuiteInfo
		{
			const std::string suiteName;
			std::chrono::microseconds suiteDuration;
			std::vector<TestMethodInfo> methods;
			const unsigned int numTests;
			unsigned int numPositiveTests;

			SuiteInfo(const std::string& name, unsigned int numTests) :
				suiteName(name), suiteDuration(std::chrono::microseconds::zero()), methods({}), numTests(numTests), numPositiveTests(0)
			{ }
		};

		std::vector<SuiteInfo> suites;
	private:
		//we need pointers to the current suite/test-method, because their name is not unique
		SuiteInfo* currentSuite = nullptr;
		TestMethodInfo* currentMethod = nullptr;
	};
} // namespace Test

#endif	/* COLLECTOROUTPUT_H */

