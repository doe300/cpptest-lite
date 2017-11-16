/* 
 * File:   cpptest-main.h
 * Author: daniel
 *
 * Created on September 12, 2015, 8:00 AM
 */

#ifndef CPPTEST_MAIN_H
#define	CPPTEST_MAIN_H

#include "cpptest.h"
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <iostream>
#include <iomanip>

namespace Test
{
	using SuiteSupplier = std::function<Test::Suite*(void)>;

	static std::map<std::string, SuiteSupplier> availableSuites;
	static std::map<std::string, std::string> suiteDescriptions;
	static bool continueAfterFailure = true;

	void setContinueAfterFail(bool continueAfterFail)
	{
		Test::continueAfterFailure = continueAfterFail;
	}

	/*!
	 * Registers a new test-suite for execution.
	 *
	 * \param supplier A function returning the test-suite instance, e.g. the constructor
	 *
	 * \param parameterName The parameter-name to be used to run this test-suite, cannot be empty or contain spaces
	 */
	void registerSuite(const SuiteSupplier supplier, const std::string& parameterName, const std::string& description = "")
	{
		if(parameterName.empty())
			throw std::invalid_argument("Test-suite parameter cannot be empty!");
		if(parameterName.find_first_of(" \"'") != std::string::npos)
			throw std::invalid_argument(std::string("Test-suite parameter is invalid: ") + parameterName);
		availableSuites[std::string("--") + parameterName] = supplier;
		suiteDescriptions[std::string("--") + parameterName] = description;
	}

	void printHelp(const std::string& progName)
	{
		std::cout << "Runs the test-suites. The list of all available test-suites can be found beneath. Passing no argument runs all available suites." << std::endl;
		std::cout << "Run with '" << progName << " [options]'" << std::endl;
		std::cout << std::endl;
		std::cout << std::setw(20) << "--help" << std::setw(4) << " " << "Prints this help" << std::endl;
		std::cout << std::setw(20) << "--output=val" << std::setw(4) << " " << "Sets the output of the tests. Available options are: plain, colored, gcc, msvc, generic. Defaults to 'plain'" << std::endl;
		std::cout << std::setw(20) << "-o=val" << std::setw(4) << " " << "'plain' prints simple text, 'colored' uses console colors, 'gcc', 'msvc' and 'generic' use compiler-like output syntax" << std::endl;
		std::cout << std::setw(20) << "--mode=val" << std::setw(4) << " " << "Sets the output mode to one of 'debug', 'verbose', 'terse' in order of the amount of information printed. Defaults to 'terse'" << std::endl;
		std::cout << std::endl;
		std::cout << "Test suites:" << std::endl;
		for(const auto& pair : suiteDescriptions)
		{
			std::cout << std::setw(20) << pair.first << std::setw(4) << " " << pair.second << std::endl;
		}
	}

	int runSuites(int argc, char** argv)
	{
		std::vector<std::unique_ptr<Test::Suite>> selectedSuites;
		selectedSuites.reserve(argc);
		std::string outputMode = "plain";
		unsigned int mode = Test::TextOutput::Terse;
		for(int i = 1; i < argc; ++i)
		{
			std::string arg(argv[i]);
			if(arg.compare("--help") == 0 || arg.compare("-h") == 0)
			{
				printHelp(argv[0]);
				return 0;
			}
			else if(arg.find("--output") == 0 || arg.find("-o") == 0)
			{
				outputMode = arg;
			}
			else if(arg.find("--mode") == 0)
			{
				if(arg.find("debug") != std::string::npos)
					mode = Test::TextOutput::Debug;
				else if(arg.find("verbose") != std::string::npos)
					mode = Test::TextOutput::Verbose;
				else if(arg.find("terse") != std::string::npos)
					mode = Test::TextOutput::Terse;
				else
					std::cerr << "Unrecognized output mode: " << arg << std::endl;
			}
			else
			{
				auto it = availableSuites.find(arg);
				if(it == availableSuites.end())
				{
					std::cerr << "Unknown parameter: " << argv[i] << std::endl;
					continue;
				}
				selectedSuites.emplace_back(it->second());
			}

		}

		if(selectedSuites.empty())
		{
			//only the program-name run all suites
			for(const auto& pair : availableSuites)
			{
				selectedSuites.emplace_back(pair.second());
			}
		}

		std::unique_ptr<Test::Output> realOutput;
		std::unique_ptr<Test::Output> output;

		if(outputMode.find("plain") != std::string::npos)
			realOutput.reset(new Test::TextOutput(mode));
		else if(outputMode.find("colored") != std::string::npos)
			realOutput.reset(new Test::ConsoleOutput(mode));
		else if(outputMode.find("gcc") != std::string::npos)
			realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_GCC));
		else if(outputMode.find("msvc") != std::string::npos)
			realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_MSVC));
		else if(outputMode.find("generic") != std::string::npos)
			realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_GENERIC));
		else
		{
			std::cout << "Unrecognized output: " << outputMode << std::endl;
			realOutput.reset(new Test::TextOutput(mode));
		}

		//there could be parallel suites
		output.reset(new Test::SynchronizedOutput(*realOutput));

		if(selectedSuites.empty())
		{
			std::cerr << "No Test-suites selected, exiting!" << std::endl;
			return 0;
		}

		bool failures = false;
		for(auto& suite : selectedSuites)
		{
			failures = !suite->run(*output, Test::continueAfterFailure) || failures;
		}

		return failures ? 1 : 0;
	}

	template<typename T>
	Test::Suite* newInstance()
	{
		return new T();
	}
}

#endif	/* CPPTEST_MAIN_H */

