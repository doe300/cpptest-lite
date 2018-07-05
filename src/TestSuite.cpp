/* 
 * File:   SimpleTest.cpp
 * Author: daniel
 * 
 * Created on September 10, 2015, 11:28 AM
 */

#include "TestSuite.h"

#include <exception>
#include <iostream>

using namespace Test;

unsigned int Test::Suite::totalTestMethods = 0;

Suite::Suite() : Suite("")
{ }

Suite::Suite(const std::string& suiteName) : suiteName(suiteName), testMethods({}), subSuites({}),
		currentTestMethodName(""), currentTestMethodArgs(""), totalDuration(std::chrono::microseconds::zero()), output(nullptr), positiveTestMethods(0), continueAfterFail(true), currentTestSucceeded(false)
{ }

void Suite::add(std::shared_ptr<Test::Suite> suite)
{
	subSuites.push_back(suite);
}

bool Suite::run(Output& output, bool continueAfterFail)
{
	this->continueAfterFail = continueAfterFail;
	this->output = &output;
	output.initializeSuite(suiteName, static_cast<unsigned>(testMethods.size()));
	//run tests
	totalDuration = std::chrono::microseconds::zero();
	positiveTestMethods = 0;
	//run setup before all tests
	if(setup())
	{
		for(const TestMethod& method : testMethods)
		{
			std::pair<bool, std::chrono::microseconds> result = runTestMethod(method);
			totalDuration += result.second;
			if(result.first) ++positiveTestMethods;
		}
		//run tear-down after all tests
		tear_down();
	}
	output.finishSuite(suiteName, static_cast<unsigned>(testMethods.size()), positiveTestMethods, totalDuration);

	//run sub-suites
	for(std::shared_ptr<Test::Suite>& suite : subSuites)
	{
		suite->run(output, continueAfterFail);
	}

	return positiveTestMethods == testMethods.size();
}

std::pair<bool, std::chrono::microseconds> Suite::runTestMethod(const TestMethod& method)
{
	errno = 0;
	bool exceptionThrown = false;
	currentTestMethodName = method.name;
	currentTestMethodArgs = method.argString;
	currentTestSucceeded = true;
	output->initializeTestMethod(suiteName, method.name, method.argString);
	//run before() before every test
	if(before(currentTestMethodName))
	{
		std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
		try {
			method(static_cast<Suite*>(this));
		}
		catch(const std::exception& e)
		{
			exceptionThrown = true;
			currentTestSucceeded = false;
			output->printException(suiteName, method.name, method.argString, e);
		}
		catch(...)
		{
			exceptionThrown = true;
			currentTestSucceeded = false;
			output->printException(suiteName, method.name, method.argString, std::runtime_error("non-exception type thrown"));
		}
		std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
		//run after() after every test
		after(currentTestMethodName, currentTestSucceeded);
		if(!exceptionThrown)
		{
			//we don't need to print twice, that the method has failed
			output->finishTestMethod(suiteName, method.name, method.argString, currentTestSucceeded);
		}
		return std::make_pair(currentTestSucceeded, std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime));
	}
	return std::make_pair(false, std::chrono::microseconds::zero());
}



// explicit instantiation of std::string needed, otherwise we get a linker error with clang on osx
// thats a bug in libc++, because of interaction with __attribute__ ((__visibility__("hidden"), __always_inline__)) in std::string
// see https://github.com/rttrorg/rttr/blob/master/src/rttr/detail/misc/standard_types.cpp
// and https://stackoverflow.com/questions/48273190/undefined-symbol-error-for-stdstringempty-c-standard-method-linking-error?rq=1
template class std::basic_string<char>;
