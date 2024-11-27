#pragma once

#include "cpptest.h"

#include <functional>
#include <string>

namespace Test {
  using SuiteSupplier = std::function<Test::Suite *(void)>;

  void setContinueAfterFail(bool continueAfterFail);
  void ignoreArgument(const std::string &arg);

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
  void registerSuite(const SuiteSupplier &supplier, const std::string &parameterName,
      const std::string &description = "", bool runByDefault = true);

  enum class RegistrationFlags : uint32_t {
    NONE = 0x00,
    /*! Do not run this test-suite in default mode (if no tests are selected explicitly when invoking the test harness)
     */
    OMIT_FROM_DEFAULT = 0x01,
    /*! Do not include this test-suite when listing tests by invoking this suite's TestSuite#listTests member function
     */
    OMIT_LIST_TESTS = 0x02,
  };

  constexpr RegistrationFlags operator|(RegistrationFlags one, RegistrationFlags other) noexcept {
    return static_cast<RegistrationFlags>(static_cast<uint32_t>(one) | static_cast<uint32_t>(other));
  }

  void registerSuite(const SuiteSupplier &supplier, const std::string &parameterName, const std::string &description,
      RegistrationFlags flags);

  void printHelp(const std::string &progName);

  /*!
   * Callback that is invoked when a command-line argument unknown to cpptest-lite is encountered.
   *
   * Using this callback allows the test program to define and handle additional command-line arguments.
   *
   * The callback returns whether the argument is handled by it (and therefore is a valid argument)
   */
  using ArgumentCallback = std::function<bool(const std::string &)>;

  int runSuites(int argc, char **argv, const ArgumentCallback &callback = nullptr);

  template <typename T>
  Test::Suite *newInstance() {
    return new T();
  }
} // namespace Test
