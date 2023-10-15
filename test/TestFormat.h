/*
 * File:   TestFormat.h
 * Author: doe300
 *
 * Created on March 4, 2017, 11:39 AM
 */

#ifndef TESTFORMAT_H
#define TESTFORMAT_H

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

#endif /* TESTFORMAT_H */

