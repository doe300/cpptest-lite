#pragma once

#include "Output.h"
#include <iostream>

namespace Test {

  /*!
   * Output-class printing failed tests and exceptions in compiler-styled text
   *
   * Any string can be passed as the format-string.
   * The format-arguments are as follows:
   * - \e %file Prints the file-name of the failed test method
   * - \e %line The line number of the failed test
   * - \e %text The error-message for the failed test
   */
  class CompilerOutput : public Output {
  public:
    static const std::string FORMAT_GCC;
    static const std::string FORMAT_MSVC;
    static const std::string FORMAT_GENERIC;

    explicit CompilerOutput(const std::string &format);
    CompilerOutput(const std::string &format, std::ostream &stream);
    CompilerOutput(const CompilerOutput &) = delete;
    CompilerOutput(CompilerOutput &&) noexcept = delete;
    ~CompilerOutput() override;

    CompilerOutput &operator=(const CompilerOutput &) = delete;
    CompilerOutput &operator=(CompilerOutput &&) = delete;

    void printException(const std::string &suiteName, const std::string &methodName, const std::string &argString,
        const std::exception &ex) override;

    void printFailure(const Assertion &assertion) override;

  private:
    const std::string format;
    std::ostream &stream;

    static const std::string ARG_FILE;
    static const std::string ARG_LINE;
    static const std::string ARG_TEXT;
  };
} // namespace Test
