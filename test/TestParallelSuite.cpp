#include "TestParallelSuite.h"

TestParallelSuite::TestParallelSuite() : Test::ParallelSuite("TestParallel")
{
    add(std::shared_ptr<Test::Suite>(new FailTestSuite()));
    add(std::shared_ptr<Test::Suite>(new CompareTestSuite()));
    add(std::shared_ptr<Test::Suite>(new ThrowTestSuite()));
    add(std::shared_ptr<Test::Suite>(new TestMacros()));
}
