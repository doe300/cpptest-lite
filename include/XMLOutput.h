#pragma once

#include "CollectorOutput.h"

#include <memory>
#include <ostream>

namespace Test {

  /*!
   * Outputs the test results in JUnit XML format supported by most tools
   */
  class XMLOutput : public CollectorOutput {
  public:
    XMLOutput(std::ostream &stream);
    XMLOutput(const std::string &outputFile);
    XMLOutput(const XMLOutput &) = delete;
    XMLOutput(XMLOutput &&) = default; // RPi cross-compiler throws on noexcept here
    ~XMLOutput() noexcept override;

    XMLOutput &operator=(const XMLOutput &) = delete;
    XMLOutput &operator=(XMLOutput &&) = delete; // RPi cross-compiler throws on noexcept here

    void finishSuite(const std::string &suiteName, unsigned int numTests, unsigned int numPositiveTests,
        std::chrono::microseconds totalDuration) override;

  private:
    std::unique_ptr<std::ostream> fileStream;
    std::ostream &output;
  };
} // namespace Test
