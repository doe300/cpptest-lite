#include "XMLOutput.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace Test;

XMLOutput::XMLOutput(std::ostream& stream) : output(stream)
{
	output << "<?xml version=\"1.0\" ?>\n<testsuites>\n";
}

XMLOutput::XMLOutput(const std::string& outputFile) : fileStream(new std::ofstream(outputFile)), output(*fileStream)
{
	output << "<?xml version=\"1.0\" ?>\n<testsuites>\n";
}

XMLOutput::~XMLOutput() noexcept
{
	output<< "</testsuites>\n";
	output.flush();
}

static std::string escapeQuotes(std::string text)
{
	std::size_t pos = 0;
	while ((pos = text.find('"', pos)) != std::string::npos)
	{
		text.replace(pos, 1, "\\\"");
		pos += 2;
	}
	return text;
}

void XMLOutput::finishSuite(const std::string& suiteName, unsigned int numTests, unsigned int numPositiveTests, std::chrono::microseconds totalDuration)
{
	CollectorOutput::finishSuite(suiteName, numTests, numPositiveTests, totalDuration);

	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(totalDuration);
	std::chrono::microseconds remainder = totalDuration - seconds;
	unsigned numErrors = std::count_if(currentSuite->methods.begin(), currentSuite->methods.end(), [] (const TestMethodInfo& method) { return !method.exceptionMessage.empty(); });
	output << "\t<testsuite name=\"" << suiteName << "\" tests=\"" << numTests << "\" failures=\""
		<< (numTests - numPositiveTests - numErrors) << "\" errors=\"" << numErrors << "\" time=\""
		<< seconds.count() << '.' << std::setfill('0') << std::setw(3) << remainder.count() << "\" timestamp=\""
		<< std::put_time(gmtime(&now), "%FT%T") << "\">\n";

	for(const TestMethodInfo& method : currentSuite->methods)
	{
		std::string name = stripMethodName(method.methodName);
		if(!method.argString.empty())
			name.append("(" + method.argString + ")");
		output << "\t\t<testcase classname=\"" << suiteName << "\" name=\"" << escapeQuotes(name) << "\">\n";
		if(!method.exceptionMessage.empty())
			output << "\t\t\t<error message=\"" << escapeQuotes(method.exceptionMessage) << "\" type=\"\"/>\n";
		else if(method.failedAssertions.empty() && method.passedAssertions.empty())
			output << "\t\t\t<skipped message=\"Test case has no assertions\" type=\"\"/>\n";
		else if(!method.failedAssertions.empty())
		{
			output << "\t\t\t<failure message=\"" << method.failedAssertions.size() << " assertions failed\" type=\"\">\n";
			for(const Assertion& assertion : method.failedAssertions)
			{
				output << "\t\t\t\tFailure: " << assertion.errorMessage << '\n';
				output << "\t\t\t\tFile: " << Private::getFileName(assertion.file) << '\n';
				output << "\t\t\t\tLine: " << assertion.lineNumber << '\n';
				if(!assertion.userMessage.empty())
					output << "\t\t\t\tMessage: " << assertion.userMessage << '\n';
				output << '\n';
			}
			output << "\t\t\t</failure>\n";
		}
		output << "\t\t</testcase>\n";
	}

	output << "\t</testsuite>\n";
}
