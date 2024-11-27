#include "TestSuite.h"

#include <exception>
#include <iostream>
#if defined(__GNUG__) || defined(__clang__)
#include <cxxabi.h>
#endif

using namespace Test;

Suite::Suite() : Suite("") {}

Suite::Suite(const std::string &suiteName)
    : suiteName(suiteName), testMethods({}), subSuites({}), currentTestMethodName(""), currentTestMethodArgs(""),
      totalDuration(std::chrono::microseconds::zero()), output(nullptr), positiveTestMethods(0),
      continueAfterFail(true), currentTestSucceeded(false) {}

void Suite::add(const std::shared_ptr<Test::Suite> &suite) { subSuites.push_back(suite); }

bool Suite::run(Output &output, bool continueAfterFail) { return run(output, listTests(), continueAfterFail); }

bool Suite::run(Output &output, const std::vector<TestMethodInfo> &selectedMethods, bool continueAfterFail) {
  auto selectedTestMethods = filterTests(selectedMethods);

  this->continueAfterFail = continueAfterFail;
  this->output = &output;
  output.initializeSuite(suiteName, static_cast<unsigned>(selectedTestMethods.size()));
  // run tests
  totalDuration = std::chrono::microseconds::zero();
  positiveTestMethods = 0;
  // run setup before all tests
  if (setup()) {
    for (const auto &method : selectedTestMethods) {
      std::pair<bool, std::chrono::microseconds> result = runTestMethod(method.get());
      totalDuration += result.second;
      if (result.first)
        ++positiveTestMethods;
    }
    // run tear-down after all tests
    tear_down();
  }
  output.finishSuite(suiteName, static_cast<unsigned>(selectedTestMethods.size()), positiveTestMethods, totalDuration);

  // run sub-suites
  for (std::shared_ptr<Test::Suite> &suite : subSuites) {
    suite->run(output, selectedMethods, continueAfterFail);
  }

  return positiveTestMethods == selectedTestMethods.size();
}

std::vector<TestMethodInfo> Suite::listTests() const {
  std::vector<TestMethodInfo> result;
  result.reserve(testMethods.size());
  for (const auto &method : testMethods) {
    result.emplace_back(
        TestMethodInfo{reinterpret_cast<std::uintptr_t>(&method), method.name + "(" + method.argString + ")"});
  }
  for (const auto &suite : subSuites) {
    auto tmp = suite->listTests();
    result.insert(result.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
  }
  return result;
}

std::pair<bool, std::chrono::microseconds> Suite::runTestMethod(const TestMethod &method) {
  errno = 0;
  bool exceptionThrown = false;
  currentTestMethodName = method.name;
  currentTestMethodArgs = method.argString;
  currentTestSucceeded = true;
  output->initializeTestMethod(suiteName, method.name, method.argString);
  // run before() before every test
  if (before(currentTestMethodName)) {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    try {
      method(static_cast<Suite *>(this));
    } catch (const AssertionFailedException &afe) {
      currentTestSucceeded = false;
    } catch (const std::exception &e) {
      exceptionThrown = true;
      currentTestSucceeded = false;
      output->printException(suiteName, method.name, method.argString, e);
    } catch (...) {
      exceptionThrown = true;
      currentTestSucceeded = false;
      output->printException(suiteName, method.name, method.argString, std::runtime_error("non-exception type thrown"));
    }
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    // run after() after every test
    after(currentTestMethodName, currentTestSucceeded);
    if (!exceptionThrown) {
      // we don't need to print twice, that the method has failed
      output->finishTestMethod(suiteName, method.name, method.argString, currentTestSucceeded);
    }
    return std::make_pair(
        currentTestSucceeded, std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime));
  }
  return std::make_pair(false, std::chrono::microseconds::zero());
}

std::vector<std::reference_wrapper<const Suite::TestMethod>> Suite::filterTests(
    const std::vector<TestMethodInfo> &selectedMethods) {
  std::vector<std::reference_wrapper<const TestMethod>> result;
  result.reserve(selectedMethods.size());

  for (const auto &info : selectedMethods) {
    const auto *method = reinterpret_cast<const TestMethod *>(info.reference);
    result.emplace_back(std::cref(*method));
  }

  return result;
}

std::string Suite::toPrettyTypeName(const std::type_info &type) {
  // Adapted from Howard Hinnants's implementation in http://stackoverflow.com/a/18369732
  std::string result = type.name();
#if defined(__GNUG__) || defined(__clang__)
  std::unique_ptr<char, void (*)(void *)> owner{abi::__cxa_demangle(type.name(), nullptr, nullptr, nullptr), std::free};
  result = owner ? owner.get() : type.name();
#endif
  return result;
}

// explicit instantiation of std::string needed, otherwise we get a linker error with clang on osx
// thats a bug in libc++, because of interaction with __attribute__ ((__visibility__("hidden"), __always_inline__)) in
// std::string see https://github.com/rttrorg/rttr/blob/master/src/rttr/detail/misc/standard_types.cpp and
// https://stackoverflow.com/questions/48273190/undefined-symbol-error-for-stdstringempty-c-standard-method-linking-error?rq=1
template class std::basic_string<char>;
