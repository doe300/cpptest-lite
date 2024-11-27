#pragma once

#include <chrono>
#include <exception>
#include <string>

namespace Test {
  namespace Private {

    /*!
     * Returns the file-name without any folders extracted from the given file-path
     */
    std::string getFileName(const std::string &file);
  } // namespace Private

  struct Assertion {
    std::string suite;
    const std::string file;
    std::string method;
    std::string args;
    std::string errorMessage;
    const std::string userMessage;
    const int lineNumber;

    Assertion(const char *fileName, int lineNumber, const std::string &errorMessage, const std::string &userMessage);
    Assertion(const char *fileName, int lineNumber, const std::string &errorMessage, const char *userMessage);
    Assertion(const char *fileName, int lineNumber, const std::string &userMessage);
    Assertion(const char *fileName, int lineNumber, const char *userMessage);
    Assertion(const char *fileName, int lineNumber);
  };

  /*!
   * Base class for all kinds of Outputs
   */
  class Output {
  public:
    Output(const Output &orig) = delete;
    Output(Output &&) noexcept = default;
    virtual ~Output() noexcept = default;

    Output &operator=(const Output &) = delete;
    Output &operator=(Output &&) noexcept = default;

    /*!
     * Initializes the output for a single suite
     *
     * \param suiteName The name of the suite
     * \param numTests The number of tests in this suite
     */
    virtual void initializeSuite(const std::string &suiteName, unsigned int numTests) {
      (void)suiteName;
      (void)numTests;
    }

    /*!
     * Finishes the output for a single suite
     *
     * \param suiteName The name of the suite
     * \param numTests The number of tests in the suite
     * \param numPositiveTests The number of successful tests in this suite
     * \param totalDuration The total duration in microseconds the suite took to execute
     */
    virtual void finishSuite(const std::string &suiteName, unsigned int numTests, unsigned int numPositiveTests,
        std::chrono::microseconds totalDuration) {
      (void)suiteName;
      (void)numTests;
      (void)numPositiveTests;
      (void)totalDuration;
    }

    /*!
     * Initializes the output for a single test-method
     *
     * \param suiteName The name of the suite
     * \param methodName The name of the test-method
     * \param argString The string-representation of the argument
     */
    virtual void initializeTestMethod(
        const std::string &suiteName, const std::string &methodName, const std::string &argString) {
      (void)suiteName;
      (void)methodName;
      (void)argString;
    }

    /*!
     * Finished the output for a single test-method
     *
     * \param suiteName The name of the suite
     * \param methodName The name of the test-method
     * \param argString The argument-string for the test-method
     * \param withSuccess Whether the test-method was finished with success
     */
    virtual void finishTestMethod(
        const std::string &suiteName, const std::string &methodName, const std::string &argString, bool withSuccess) {
      (void)suiteName;
      (void)methodName;
      (void)argString;
      (void)withSuccess;
    }

    /*!
     * Finished the output for a single test-method that threw an error
     *
     * NOTE: when this method is called, \ref finishTestMethod is skipped for this test-method
     *
     * \param suiteName The name of the suite
     * \param methodName The name of the test-method
     * \param argString The argument-string for the test-method
     * \param ex The exception, that was thrown
     */
    virtual void printException(const std::string &suiteName, const std::string &methodName,
        const std::string &argString, const std::exception &ex) {
      (void)suiteName;
      (void)methodName;
      (void)argString;
      (void)ex;
    }

    /*!
     * Prints a successful test
     *
     * NOTE: this method is only called, if the macro CPPTEST_LITE_LOG_SUCCESS is set
     *
     * \param assertion Information about the successful assertion
     */
    virtual void printSuccess(const Assertion &assertion) { (void)assertion; }

    /*!
     * Prints a failed test
     *
     * \param assertion Information about the failed assertion
     */
    virtual void printFailure(const Assertion &assertion) { (void)assertion; }

  protected:
    Output() = default;

    /*!
     * Converts a fraction into a percentage-value with 2 decimals
     */
    double prettifyPercentage(const double part, const double whole) const;

    /*!
     * Removes the class-name from the given methodName
     */
    std::string stripMethodName(const std::string &methodName) const;

    /*!
     * Truncates the string to the given length and adding trailing "..." if necessary
     */
    std::string truncateString(const std::string &string, unsigned int length) const;
  };

} // namespace Test
