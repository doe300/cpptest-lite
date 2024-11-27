#include <iostream>

#include "../include/cpptest-main.h"
#include "TestAssertions.h"
#include "TestBDD.h"
#include "TestFormat.h"
#include "TestMacros.h"
#include "TestOutputs.h"
#include "TestParallelSuite.h"

using namespace std;

// Main test program
//
int main(int argc, char *argv[]) {
  // TODO required for proper UTF-16 to locale-dependent multi-byte conversion (in formatting tests)
  setlocale(LC_ALL, "en_US.utf8");

  Test::setContinueAfterFail(true);
  Test::ignoreArgument("--no-such-argument");
  Test::registerSuite(Test::newInstance<FailTestSuite>, "fail-tests", "Tests the correct handling of failed tests");
  Test::registerSuite(Test::newInstance<CompareTestSuite>, "test-comparisons", "Tests the comparison tests");
  Test::registerSuite(Test::newInstance<ThrowTestSuite>, "test-exceptions",
      "Tests correct handling of exceptions (expected and unexpected)");
  Test::registerSuite(Test::newInstance<TestMacros>, "test-macros", "Tests all available test-macros");
  Test::registerSuite(Test::newInstance<TestFormat>, "test-format", "Tests the various output formats");
  Test::registerSuite(Test::newInstance<TestOutputs>, "test-outputs", "Tests the various output types",
      Test::RegistrationFlags::OMIT_LIST_TESTS);
  Test::registerSuite(Test::newInstance<TestParallelSuite>, "test-parallel", "Tests the parallel test suite");
  Test::registerSuite(Test::newInstance<TestAssertions>, "test-assertions", "Tests the available TEST_XXX assertions");
  Test::registerSuite(
      Test::newInstance<Story1>, "story1", "Runs the first BDD story", Test::RegistrationFlags::OMIT_FROM_DEFAULT);
  Test::registerSuite(Test::newInstance<Story2>, "story2", "Runs the second BDD story");
  Test::registerSuite(Test::newInstance<Story3>, "story3", "Runs the third BDD story");

  return Test::runSuites(argc, argv);
}
