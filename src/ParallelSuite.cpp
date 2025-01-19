#include "ParallelSuite.h"

using namespace Test;

ParallelSuite::ParallelSuite(const std::string &name) : Suite(name) {}
ParallelSuite::~ParallelSuite() { delete output; }

bool ParallelSuite::run(Output &out, const std::vector<TestMethodInfo> &selectedMethods, bool continueOnError) {
  this->continueAfterFail = continueOnError;
  this->output = new SynchronizedOutput(out);
  out.initializeSuite(suiteName, static_cast<unsigned>(testMethods.size()));
  if (setup()) {
    // warn if test-methods are directly added
    if (!testMethods.empty()) {
      Assertion notEmptyAssterion(
          suiteName.c_str(), 0, "Any test-methods directly added to a parallel-suite are not executed!");
      out.printFailure(notEmptyAssterion);
    }
    // run tear-down after all tests
    tear_down();
  }
  out.finishSuite(suiteName, static_cast<unsigned>(testMethods.size()), 0, std::chrono::microseconds::zero());

  std::vector<std::future<bool>> results(subSuites.size());
  // run sub-suites
  for (unsigned int i = 0; i < subSuites.size(); i++) {
    results[i] = std::async(std::launch::async, &ParallelSuite::runSuite, this, i, selectedMethods);
  }

  // join sub-suites
  for (std::future<bool> &result : results) {
    result.get();
  }

  return true;
}

bool ParallelSuite::runSuite(unsigned int suiteIndex, const std::vector<TestMethodInfo> &selectedMethods) {
  return subSuites[suiteIndex]->run(*output, selectedMethods, continueAfterFail);
}
