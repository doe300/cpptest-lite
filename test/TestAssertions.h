#pragma once

#include "cpptest.h"

/*
 * Contains only positive tests in contrast to other test suites mainly testing failing behavior
 */
class TestAssertions : public Test::Suite
{
public:
    TestAssertions();

    void testAssertBoolean();
    void testAssertEquals();
    void testAssertDelta();
    void testAssertULP();
    void testThrows();
    void testPredicate();
    void testBiPredicate();
    void testAssertNotEquals();
    void testAssertFalse();
};
