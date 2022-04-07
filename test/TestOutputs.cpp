/*
 * File:   TestOutputs.cpp
 * Author: daniel
 *
 * Created on September 20, 2015, 12:41 PM
 */

#include "TestOutputs.h"

#include <fstream>

using namespace Test;

TestOutputs::TestOutputs() : Test::Suite("TestOutputs"), textOutput(new TextOutput(TextOutput::Verbose)),
        compilerOutput(new CompilerOutput(CompilerOutput::FORMAT_GENERIC)), htmlOutput(new HTMLOutput()),
        consoleOutput(new ConsoleOutput(TextOutput::Verbose)), xmlOutput(new XMLOutput("result.xml"))
{
    TEST_ADD_WITH_POINTER(TestOutputs::testOutput, static_cast<void*>(textOutput.get()));
    TEST_ADD_WITH_POINTER(TestOutputs::testOutput, static_cast<void*>(compilerOutput.get()));
    TEST_ADD_WITH_POINTER(TestOutputs::testOutput, static_cast<void*>(htmlOutput.get()));
    TEST_ADD_WITH_POINTER(TestOutputs::testOutput, static_cast<void*>(consoleOutput.get()));
    TEST_ADD_WITH_POINTER(TestOutputs::testOutput, static_cast<void*>(xmlOutput.get()));
}

TestOutputs::~TestOutputs() = default;


void TestOutputs::testOutput(void* output)
{
    Output* realOutput = static_cast<Output*>(output);
    TestWithOutput runTest;
    runTest.run(*realOutput, true);
    HTMLOutput* htmlOutput = dynamic_cast<HTMLOutput*>(realOutput);
    if(htmlOutput != nullptr)
    {
        std::fstream htmlFile("result.html", std::ios_base::out);
        htmlOutput->generate(htmlFile, true);
        htmlFile.close();
    }
}

TestWithOutput::TestWithOutput() : Suite("TestWithOutput")
{
    // test Output-format
    TEST_ADD(TestWithOutput::someTestMethod);
    // test overloaded test-methods
    TEST_ADD_WITH_STRING_LITERAL(TestWithOutput::anotherTestMethod, const_cast<char*>(std::string("Test method 1").data()));
    TEST_ADD_WITH_STRING_LITERAL(TestWithOutput::anotherTestMethod, const_cast<char*>(std::string("Test method 2").data()));
    // test successful test method
    TEST_ADD(TestWithOutput::somePassingMethod);
    TEST_ADD(TestWithOutput::emptyMethod);
}

void TestWithOutput::someTestMethod()
{
    TEST_ASSERT(false);
    TEST_ASSERT_EQUALS(1, 4);

    TEST_FAIL("Failed test");

    TEST_ASSERT_MSG(true, "Passes");
}

void TestWithOutput::anotherTestMethod(char* someText)
{
    TEST_ASSERT_MSG(std::string(someText).empty(), "Should fail, text is not empty");

    TEST_ASSERT(std::string(someText).substr(0, 4) == "Test");
    TEST_ASSERT_MSG(someText[12] == '1', "Fails in one case");
}

void TestWithOutput::somePassingMethod()
{
    TEST_ASSERT(true);
    TEST_ASSERT_EQUALS(1u, uint16_t{1});
}

void TestWithOutput::emptyMethod()
{
    // no content
}
