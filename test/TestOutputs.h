/* 
 * File:   TestOutputs.h
 * Author: daniel
 *
 * Created on September 20, 2015, 12:41 PM
 */

#ifndef TESTOUTPUTS_H
#define	TESTOUTPUTS_H

#include "cpptest.h"

class TestOutputs : public Test::Suite
{
public:
    TestOutputs();
    ~TestOutputs() override;
    
    void testOutput(void* output);
private:
    Test::Output* textOutput;
    Test::Output* compilerOutput;
    Test::Output* htmlOutput;
    Test::Output* consoleOutput;
};

class TestWithOutput : public Test::Suite
{
public:
    TestWithOutput();
    
    void someTestMethod();
    
    void anotherTestMethod(char* someText);
};

#endif	/* TESTOUTPUTS_H */

