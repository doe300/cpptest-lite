#include "Output.h"

using namespace Test;
std::string Private::getFileName(const std::string &file) {
  std::string fileName = file;
  std::size_t pos = fileName.find_last_of('\\');
  if (pos != std::string::npos)
    fileName = fileName.substr(pos + 1);
  pos = fileName.find_last_of('/');
  if (pos != std::string::npos)
    fileName = fileName.substr(pos + 1);
  return fileName;
}

Assertion::Assertion(
    const char *fileName, uint32_t lineNum, const std::string &errorMsg, const std::string &userMsg)
    : suite(""), file(fileName), method(""), args(""), errorMessage(errorMsg), userMessage(userMsg),
      lineNumber(lineNum) {}

Assertion::Assertion(const char *fileName, uint32_t lineNum, const std::string &errorMsg, const char *userMsg)
    : Assertion(fileName, lineNum, errorMsg, std::string{userMsg != nullptr ? userMsg : ""}) {}

Assertion::Assertion(const char *fileName, uint32_t lineNum, const std::string &userMsg)
    : suite(""), file(fileName), method(""), args(""), errorMessage(""), userMessage(userMsg),
      lineNumber(lineNum) {}

Assertion::Assertion(const char *fileName, uint32_t lineNum, const char *userMsg)
    : Assertion(fileName, lineNum, std::string{userMsg != nullptr ? userMsg : ""}) {}

Assertion::Assertion(const char *fileName, uint32_t lineNum)
    : suite(""), file(fileName), method(""), args(""), errorMessage(""), userMessage(""), lineNumber(lineNum) {}

double Output::prettifyPercentage(const double part, const double whole) const {
  if (part == 0 || whole == 0) {
    return 0;
  }
  int tmp = static_cast<int>((part / whole) * 10000);
  return tmp / 100.0;
}

std::string Output::stripMethodName(const std::string &methodName) const {
  std::size_t pos = methodName.size() - 1;
  while (pos > 0) {
    if (methodName[pos] == ':')
      return methodName.substr(pos + 1);
    --pos;
  }
  return methodName;
}

std::string Output::truncateString(const std::string &string, unsigned int length) const {
  if (string.size() <= length) {
    return string;
  }
  return string.substr(0, length - 3) + "...";
}
