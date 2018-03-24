
#include "TestAssertions.h"

TestAssertions::TestAssertions()
{
    TEST_ADD(TestAssertions::testAssertBoolean);
    TEST_ADD(TestAssertions::testAssertEquals);
    TEST_ADD(TestAssertions::testAssertDelta);
    TEST_ADD(TestAssertions::testAssertULP);
    TEST_ADD(TestAssertions::testThrows);
    TEST_ADD(TestAssertions::testPredicate);
    TEST_ADD(TestAssertions::testBiPredicate);
}

void TestAssertions::testAssertBoolean()
{
    TEST_ASSERT(true);
    TEST_ASSERT(1 == 1);
    TEST_ASSERT(1.0f == 1.0f);
}

void TestAssertions::testAssertEquals()
{
    TEST_ASSERT_EQUALS(1, 1);
    TEST_ASSERT_EQUALS(nullptr, nullptr);
    TEST_ASSERT_EQUALS(1.0f, 1.0f);
}

void TestAssertions::testAssertDelta()
{
    TEST_ASSERT_DELTA(1.0f, 1.0f, 0.0f);
    TEST_ASSERT_DELTA(1.0f, 1.1f, 0.11f);
    TEST_ASSERT_DELTA(-1.0f, -1.1f, -0.11f);
    TEST_ASSERT_DELTA(-1.0, -1.1, 0.11);
}

void TestAssertions::testAssertULP()
{
    TEST_ASSERT_ULP(1.0f, 1.0f, 0);
    TEST_ASSERT_ULP(1.0f, std::nextafter(1.0f, 2.0f), 1);
    TEST_ASSERT_ULP(1.0f, std::nextafter(1.0f, 0.0f), 1);
}

void TestAssertions::testThrows()
{
    int i, j;
    TEST_THROWS_NOTHING(std::string s);
    TEST_THROWS_NOTHING(i = i + j);
}

void TestAssertions::testPredicate()
{
    int i = 1;
    TEST_PREDICATE([](int i) -> bool { return i;}, i);
    TEST_PREDICATE([](int i) -> bool { return i > 0;}, i);
}

void TestAssertions::testBiPredicate()
{
    int i = 0, j = 1;
    TEST_BIPREDICATE((std::less<int>{}), i, j);
}