/* 
 * File:   TestMain.cpp
 * Author: 
 * 
 * Created on March 31, 2022, 2:56 PM
 */

#include "cpptest-main.h"

#include "cpptest.h"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

namespace Test
{
	using SuiteSupplier = std::function<Test::Suite*(void)>;

	struct SuiteEntry
	{
		std::string name;
		SuiteSupplier supplier;
		std::string description;
		bool runByDefault;
	};

	//using a list here sorts entries by order of insertion
	static std::vector<SuiteEntry> availableSuites;
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
	 *
	 * \param description An optional description to be printed for the --help option
	 *
	 * \param runByDefault Whether this test-suite is run in default mode (no explicit suites specified)
	 */
	void registerSuite(const SuiteSupplier& supplier, const std::string& parameterName, const std::string& description, bool runByDefault)
	{
		if(parameterName.empty())
			throw std::invalid_argument("Test-suite parameter cannot be empty!");
		if(parameterName.find_first_of(" \"'") != std::string::npos)
			throw std::invalid_argument(std::string("Test-suite parameter is invalid: ") + parameterName);
		SuiteEntry entry;
		entry.name = std::string("--") + parameterName;
		entry.supplier = supplier;
		entry.description = description;
		entry.runByDefault = runByDefault;
		availableSuites.push_back(entry);
	}

	void printHelp(const std::string& progName)
	{
		static const int paramWidth = 30;
		static const int gapWidth = 4;
		std::cout << "Runs the test-suites. The list of all available test-suites can be found beneath. Passing no argument runs all suites marked with '(default)'." << std::endl;
		std::cout << "Run with '" << progName << " [options]'" << std::endl;
		std::cout << std::endl;
		std::cout << std::setw(paramWidth) << "--help" << std::setw(gapWidth) << " " << "Prints this help" << std::endl;
		std::cout << std::setw(paramWidth) << "--output=val" << std::setw(gapWidth) << " " << "Sets the output of the tests. Available options are: plain, colored, gcc, msvc, generic. Defaults to 'plain'" << std::endl;
		std::cout << std::setw(paramWidth) << "-o=val" << std::setw(gapWidth) << " " << "'plain' prints simple text, 'colored' uses console colors, 'gcc', 'msvc' and 'generic' use compiler-like output syntax" << std::endl;
		std::cout << std::setw(paramWidth) << "--mode=val" << std::setw(gapWidth) << " " << "Sets the output mode to one of 'debug', 'verbose', 'terse' in order of the amount of information printed. Defaults to 'terse'" << std::endl;
		std::cout << std::setw(paramWidth) << "--output-file=file" << std::setw(gapWidth) << " " << "Sets the optional output file to write to, defaults to 'stdout'. 'colored' output can only write to console!" << std::endl;
		std::cout << std::endl;
		std::cout << "Test suites:" << std::endl;
		//sort suites by name
		std::map<std::string, SuiteEntry> sortedSuites;
		for(const auto& entry : availableSuites)
			sortedSuites.emplace(entry.name, entry);
		for(const auto& pair : sortedSuites)
		{
			std::cout << std::setw(paramWidth) << pair.first << std::setw(gapWidth) << " " << pair.second.description << (pair.second.runByDefault ? " (default)" : "") << std::endl;
		}
	}

	/*!
	 * Callback that is invoked when a command-line argument unknown to cpptest-lite is encountered.
	 *
	 * Using this callback allows the test program to define and handle additional command-line arguments.
	 *
	 * The callback returns whether the argument is handled by it (and therefore is a valid argument)
	 */
	using ArgumentCallback = std::function<bool(const std::string&)>;

	int runSuites(int argc, char** argv, const ArgumentCallback& callback)
	{
		std::vector<std::unique_ptr<Test::Suite>> selectedSuites;
		selectedSuites.reserve(argc);
		std::string outputMode = "plain";
		std::string outputFile;
		unsigned int mode = Test::TextOutput::Terse;
		for(int i = 1; i < argc; ++i)
		{
			std::string arg(argv[i]);
			if(arg == "--help" || arg == "-h")
			{
				printHelp(argv[0]);
				return 0;
			}
			else if(arg.find("--output-file") == 0)
			{
				if(arg.find('=') != std::string::npos)
					outputFile = arg.substr(arg.find('=') + 1);
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
				const char* name = argv[i];
				auto it = std::find_if(availableSuites.begin(), availableSuites.end(), [name](const SuiteEntry& e) -> bool {return e.name == name;});
				if(it == availableSuites.end() && !callback(arg))
				{
					std::cerr << "Unknown parameter: " << argv[i] << std::endl;
					continue;
				}
				selectedSuites.emplace_back(it->supplier());
			}

		}

		if(selectedSuites.empty())
		{
			//only the program-name run all suites
			for(const auto& entry : availableSuites)
			{
				if(entry.runByDefault)
					selectedSuites.emplace_back(entry.supplier());
			}
		}

		std::ofstream f;
		if(!outputFile.empty())
			f.open(outputFile, std::ios_base::out|std::ios_base::trunc);

		std::unique_ptr<Test::Output> realOutput;
		std::unique_ptr<Test::Output> output;

		if(outputMode.find("plain") != std::string::npos)
		{
			if(!outputFile.empty())
				realOutput.reset(new Test::TextOutput(mode, f));
			else
				realOutput.reset(new Test::TextOutput(mode));
		}
		else if(outputMode.find("colored") != std::string::npos)
			realOutput.reset(new Test::ConsoleOutput(mode));
		else if(outputMode.find("gcc") != std::string::npos)
		{
			if(!outputFile.empty())
				realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_GCC, f));
			else
				realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_GCC));
		}
		else if(outputMode.find("msvc") != std::string::npos)
		{
			if(!outputFile.empty())
				realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_MSVC, f));
			else
				realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_MSVC));
		}
		else if(outputMode.find("generic") != std::string::npos)
		{
			if(!outputFile.empty())
				realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_GENERIC, f));
			else
				realOutput.reset(new Test::CompilerOutput(Test::CompilerOutput::FORMAT_GENERIC));
		}
		else
		{
			std::cout << "Unrecognized output: " << outputMode << std::endl;
			if(!outputFile.empty())
				realOutput.reset(new Test::TextOutput(mode, f));
			else
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

} // namespace Test