/* 
 * File:   SimpleTest.h
 * Author: daniel
 *
 * Created on September 10, 2015, 11:28 AM
 */

#ifndef TESTSUITE_H
#define	TESTSUITE_H

#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include <memory> //shared_ptr

#include "Output.h"

namespace Test
{
    
class Suite
{
public:
    Suite();
    Suite(const std::string& suiteName);
    
    void add(std::shared_ptr<Test::Suite> suite);
    
    bool run(Output& output, bool continueAfterFail = true);
    
    static unsigned int getTotalNumberOfTests()
    {
        return totalTestMethods;
    }
    
protected:
    
    typedef void (Suite::*SimpleTestMethod)();
    typedef void (Suite::*StringTestMethod)(const std::string& string);
    typedef void (Suite::*NumberTestMethod)(const int number);
    typedef void (Suite::*PointerTestMethod)(const void* ptr);
    
    inline void setSuiteName(const std::string& suiteName)
    {
        if(this->suiteName.empty())
        {
            this->suiteName = Private::getFileName(suiteName);
            this->suiteName = this->suiteName.substr(0, this->suiteName.find_last_of('.'));
        }
    }
    
    inline void addTest(SimpleTestMethod method, const std::string& funcName)
    {
        testMethods.push_back(TestMethod(funcName, method));
        totalTestMethods++;
    }
    inline void addTest(StringTestMethod method, const std::string& funcName, const std::string string)
    {
        testMethods.push_back(TestMethod(funcName, method, string));
        totalTestMethods++;
    }
    inline void addTest(NumberTestMethod method, const std::string& funcName, const int number)
    {
        testMethods.push_back(TestMethod(funcName, method, number));
        totalTestMethods++;
    }
    inline void addTest(PointerTestMethod method, const std::string& funcName, const void* ptr)
    {
        testMethods.push_back(TestMethod(funcName, method, ptr));
        totalTestMethods++;
    }
    //TODO varargs-version
    
    #ifdef CPPTEST_LITE_LOG_SUCCESS
    inline void testSucceeded(Assertion assertion)
    {
        assertion.method = currentTestMethodName;
        assertion.suite = suiteName;
        output->printSuccess(assertion);
    }
    #else
    inline void testSucceeded(Assertion assertion)
    {
        //do nothing
    }
    #endif
    inline void testFailed(Assertion assertion)
    {
        currentTestSucceeded = false;
        assertion.method = currentTestMethodName;
        assertion.suite = suiteName;
        output->printFailure(assertion);
    }
    
    inline bool continueAfterFailure()
    {
        return continueAfterFail;
    }
    
    virtual void setup()
    {
        //does nothing by default
    }
    virtual void tear_down()
    {
        //does nothing by default
    }
    
    virtual void before(const std::string methodName)
    {
        //does nothing by default
    }
    
    virtual void after(const std::string methodName, const bool success)
    {
        //does nothing by default
    }
    
    static unsigned int totalTestMethods;
    
private:
    
    struct TestMethod
    {
        const std::string name;
        const std::function<void(Suite*)> functor;
        
        TestMethod() : name("")
        { }
        
        TestMethod(const std::string& name, SimpleTestMethod method) : name(name), functor(method)
        { }
        
        TestMethod(const std::string& name, StringTestMethod method, const std::string& string) : name(name), functor(std::bind(method, std::placeholders::_1, string))
        { }
        
        TestMethod(const std::string& name, NumberTestMethod method, const int param) : name(name), functor(std::bind(method, std::placeholders::_1, param))
        { }
        
        TestMethod(const std::string& name, PointerTestMethod method, const void* ptr) : name(name), functor(std::bind(method, std::placeholders::_1, ptr))
        { }
        
        inline void operator()(Suite* suite) const 
        {
            functor(suite);
        }
    };
    std::string suiteName;
    std::vector<TestMethod> testMethods;
    std::vector<std::shared_ptr<Test::Suite>> subSuites;
    bool continueAfterFail;
    
    unsigned int positiveTestMethods;
    std::string currentTestMethodName;
    bool currentTestSucceeded;
    std::chrono::microseconds totalDuration;
    Output* output;
    
    std::pair<bool, std::chrono::microseconds> runTestMethod(const TestMethod& method);
};


#define TEST_ADD(func) setSuiteName(__FILE__); addTest(static_cast<SimpleTestMethod>(&func), #func)
#define TEST_ADD_WITH_STRING(func, string) setSuiteName(__FILE__); addTest(static_cast<StringTestMethod>(&func), #func, string)
#define TEST_ADD_WITH_INTEGER(func, number) setSuiteName(__FILE__); addTest(static_cast<NumberTestMethod>(&func), #func, number)
#define TEST_ADD_WITH_POINTER(func, pointer) setSuiteName(__FILE__); addTest(static_cast<PointerTestMethod>(&func), #func, pointer)

};

#endif	/* TESTSUITE_H */

