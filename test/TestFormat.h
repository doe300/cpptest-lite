#pragma once

#include "cpptest.h"

class TestFormat : public Test::Suite
{
public:
    TestFormat();

    void testFloatingPointNumbers();
    void testStreamWriteOperator();
    void testStringTypes();
    void testMemoryRanges();
    void testTuples();
    void testNonLiteralMessages();
};
