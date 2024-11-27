#include <stdexcept>

#include "CollectorOutput.h"

using namespace Test;

void CollectorOutput::initializeSuite(const std::string &suiteName, const unsigned int numTests) {
  suites.emplace_back(suiteName, numTests);
  // get pointer to last suite
  currentSuite = &suites.back();
}

void CollectorOutput::finishSuite(const std::string &suiteName, const unsigned int numTests,
    const unsigned int numPositiveTests, const std::chrono::microseconds totalDuration) {
  if (currentSuite == nullptr)
    throw std::runtime_error("Invalid Suite!");
  currentSuite->numPositiveTests = numPositiveTests;
  currentSuite->suiteDuration = totalDuration;
}

void CollectorOutput::initializeTestMethod(
    const std::string &suiteName, const std::string &methodName, const std::string &argString) {
  if (currentSuite == nullptr)
    throw std::runtime_error("Invalid Suite!");
  currentSuite->methods.emplace_back(methodName, argString);
  // get pointer to last method
  currentMethod = &(currentSuite->methods.back());
}

void CollectorOutput::finishTestMethod(
    const std::string &suiteName, const std::string &methodName, const std::string &argString, const bool withSuccess) {
  if (currentMethod == nullptr)
    throw std::runtime_error("Invalid Test-Method!");
  currentMethod->withSuccess = withSuccess;
}

void CollectorOutput::printException(const std::string &suiteName, const std::string &methodName,
    const std::string &argString, const std::exception &ex) {
  if (currentMethod == nullptr)
    throw std::runtime_error("Invalid Test-Method!");
  currentMethod->withSuccess = false;
  currentMethod->exceptionMessage = std::string(ex.what());
}

void CollectorOutput::printSuccess(const Assertion &assertion) {
  if (currentMethod == nullptr)
    throw std::runtime_error("Invalid Test-Method!");
  currentMethod->passedAssertions.push_back(assertion);
}

void CollectorOutput::printFailure(const Assertion &assertion) {
  if (currentMethod == nullptr)
    throw std::runtime_error("Invalid Test-Method!");
  currentMethod->failedAssertions.push_back(assertion);
}
