/* 
 * File:   TestMacros.cpp
 * Author: daniel
 * 
 * Created on September 15, 2015, 6:27 PM
 */

#include <string.h>

#include "TestMacros.h"

TestMacros::TestMacros() : Test::Suite()
{
    TEST_ADD_WITH_STRING_LITERAL(TestMacros::testMethodWithStringArg, (char*)"42");
    TEST_ADD_WITH_INTEGER(TestMacros::testMethodWithIntArg, 42);
    TEST_ADD_WITH_POINTER(TestMacros::testMethodWithPointerArg, (void*)nullptr);
}

void TestMacros::testMethodWithStringArg(char* arg)
{
    TEST_ASSERT(strlen(arg) == 2);
}

void TestMacros::testMethodWithIntArg(const int arg)
{
    TEST_ASSERT_EQUALS(42, arg);
    TEST_PREDICATE([](int x){return x == 42;}, 42);
    TEST_BIPREDICATE([](int x, int y){return x == y;}, arg, arg);
}

void TestMacros::testMethodWithPointerArg(void* arg)
{
    TEST_ASSERT_EQUALS(nullptr, arg);
    TEST_ASSERT_EQUALS_MSG(nullptr, (void*)0xFF, "Should fail");
}
