#pragma once
#include "cpptest.h"

namespace Test {

  /*!
   * Test Suite for behavior driven development
   *
   * \since 0.6
   */
  class BDDSuite : public Suite {
  public:
    using Condition = std::function<bool()>;
    using Action = std::function<void()>;

    struct Scenario {
      std::string name;
      Condition precondition;
      Action action;
      Condition postcondition;
    };

    BDDSuite() = default;
    explicit BDDSuite(const std::string &name);
    BDDSuite(const BDDSuite &) = delete;
    BDDSuite(BDDSuite &&) noexcept = default;
    ~BDDSuite() noexcept override = default;

    BDDSuite &operator=(const BDDSuite &) = delete;
    BDDSuite &operator=(BDDSuite &&) = default;

    void runScenario(std::string scenarioName);

  protected:
    std::vector<Scenario> scenarios;
  };
} // namespace Test
