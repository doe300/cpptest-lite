/*
 * File:   TestOutputs.h
 * Author: daniel
 *
 * Created on September 20, 2015, 12:41 PM
 */

#ifndef TESTOUTPUTS_H
#define	TESTOUTPUTS_H

#include "cpptest.h"

#include <memory>

class TestOutputs : public Test::Suite
{
public:
    TestOutputs();
    ~TestOutputs() override;

    void testOutput(void* output);
private:
    std::unique_ptr<Test::Output> textOutput;
    std::unique_ptr<Test::Output> compilerOutput;
    std::unique_ptr<Test::Output> htmlOutput;
    std::unique_ptr<Test::Output> consoleOutput;
    std::unique_ptr<Test::Output> xmlOutput;
};

class TestWithOutput : public Test::Suite
{
public:
    TestWithOutput();

    void someTestMethod();
    void anotherTestMethod(char* someText);
    void somePassingMethod();
    void emptyMethod();
};

#endif	/* TESTOUTPUTS_H */

