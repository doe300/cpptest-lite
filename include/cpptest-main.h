/* 
 * File:   cpptest-main.h
 * Author: daniel
 *
 * Created on September 12, 2015, 8:00 AM
 */

#ifndef CPPTEST_MAIN_H
#define	CPPTEST_MAIN_H

#include "cpptest.h"

namespace Test
{
	using SuiteSupplier = std::function<Test::Suite*(void)>;

	void setContinueAfterFail(bool continueAfterFail);

	/*!
	 * Registers a new test-suite for execution.
	 *
	 * \param supplier A function returning the test-suite instance, e.g. the constructor
	 *
	 * \param parameterName The parameter-name to be used to run this test-suite, cannot be empty or contain spaces
	 *
	 * \param description An optional description to be printed for the --help option
	 *
	 * \param runByDefault Whether this test-suite is run in default mode (no explicit suites specified)
	 */
	void registerSuite(const SuiteSupplier& supplier, const std::string& parameterName, const std::string& description = "", bool runByDefault = true);

	void printHelp(const std::string& progName);

	/*!
	 * Callback that is invoked when a command-line argument unknown to cpptest-lite is encountered.
	 *
	 * Using this callback allows the test program to define and handle additional command-line arguments.
	 *
	 * The callback returns whether the argument is handled by it (and therefore is a valid argument)
	 */
	using ArgumentCallback = std::function<bool(const std::string&)>;

	int runSuites(int argc, char** argv, const ArgumentCallback& callback = [](const std::string&) -> bool { return false;});

	template<typename T>
	Test::Suite* newInstance()
	{
		return new T();
	}
} // namespace Test

#endif	/* CPPTEST_MAIN_H */

