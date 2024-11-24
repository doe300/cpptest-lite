#include "SynchronizedOutput.h"

using namespace Test;

SynchronizedOutput::SynchronizedOutput(Output& realOutput) : realOutput(realOutput)
{

}

void SynchronizedOutput::initializeSuite(const std::string& suiteName, const unsigned int numTests)
{
	std::lock_guard<std::mutex> guard(outputMutex);
	realOutput.initializeSuite(suiteName, numTests);
}

void SynchronizedOutput::finishSuite(const std::string& suiteName, const unsigned int numTests, const unsigned int numPositiveTests, const std::chrono::microseconds totalDuration)
{
	std::lock_guard<std::mutex> guard(outputMutex);
	realOutput.finishSuite(suiteName, numTests, numPositiveTests, totalDuration);
}

void SynchronizedOutput::initializeTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString)
{
	std::lock_guard<std::mutex> guard(outputMutex);
	realOutput.initializeTestMethod(suiteName, methodName, argString);
}

void SynchronizedOutput::finishTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString, const bool withSuccess)
{
	std::lock_guard<std::mutex> guard(outputMutex);
	realOutput.finishTestMethod(suiteName, methodName, argString, withSuccess);
}

void SynchronizedOutput::printException(const std::string& suiteName, const std::string& methodName, const std::string& argString, const std::exception& ex)
{
	std::lock_guard<std::mutex> guard(outputMutex);
	realOutput.printException(suiteName, methodName, argString, ex);
}

void SynchronizedOutput::printSuccess(const Assertion& assertion)
{
	std::lock_guard<std::mutex> guard(outputMutex);
	realOutput.printSuccess(assertion);
}


void SynchronizedOutput::printFailure(const Assertion& assertion)
{
	std::lock_guard<std::mutex> guard(outputMutex);
	realOutput.printFailure(assertion);
}
