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
#include <type_traits> //is_constructible

#include "Output.h"

namespace Test
{

    /*!
     * Any test-class must extend this class
     */
    class Suite
    {
    public:
        Suite();
        Suite(const std::string& suiteName);

        /*!
         * Adds another Test::Suite to this suite.
         * All associated suites will be run after this suite has finished
         * 
         */
        void add(std::shared_ptr<Test::Suite> suite);

        /*!
         * Runs all the registered test-methods in this suite
         * 
         * \param output The output to print the results to
         * \param continueAfterFail whether to continue running after a test failed
         */
        virtual bool run(Output& output, bool continueAfterFail = true);

        static unsigned int getTotalNumberOfTests()
        {
            return totalTestMethods;
        }
        
        virtual ~Suite()
        {
        }

    protected:

        //! Test-method without any parameter
        using SimpleTestMethod = void (Suite::*)();
        //! Test-method with a single parameter of arbitrary type
        template<typename T>
        using SingleArgTestMethod = void (Suite::*)(const T arg);
        template<typename... T>
        using VarargTestMethod = void (Suite::*)(T... args);

        inline void setSuiteName(const std::string& filePath)
        {
            if (this->suiteName.empty()) {
                this->suiteName = Private::getFileName(filePath);
                this->suiteName = this->suiteName.substr(0, this->suiteName.find_last_of('.'));
            }
        }

        inline void addTest(SimpleTestMethod method, const std::string& funcName)
        {
            testMethods.push_back(TestMethod(funcName, method));
            totalTestMethods++;
        }

        template<typename T, typename U>
        inline void addTest(SingleArgTestMethod<T> method, const std::string& funcName, const U& arg)
        {
            static_assert(std::is_constructible<T, U>::value, "Can't construct method-parameter out of given type!");
            testMethods.push_back(TestMethod(funcName, method, {arg}));
            totalTestMethods++;
        }
        
        template<typename... T>
        inline void addTest(VarargTestMethod<T...> method, const std::string& funcName, const T... args)
        {
            testMethods.push_back(TestMethod(funcName, method, args...));
            totalTestMethods++;
        }
        
        inline void testSucceeded(Assertion&& assertion)
        {
            assertion.method = currentTestMethodName;
            assertion.args = currentTestMethodArgs;
            assertion.suite = suiteName;
            output->printSuccess(assertion);
        }

        inline void testFailed(Assertion&& assertion)
        {
            currentTestSucceeded = false;
            assertion.method = currentTestMethodName;
            assertion.args = currentTestMethodArgs;
            assertion.suite = suiteName;
            output->printFailure(assertion);
        }

        inline bool continueAfterFailure()
        {
            return continueAfterFail;
        }

        /*!
         * This method can be overridden to execute code before the test-methods in this class are run
         * 
         * \return A boolean value, if false, the whole suite will not be executed (inclusive \ref tearDown)
         */
        virtual bool setup()
        {
            //does nothing by default
            return true;
        }

        /*!
         * This method can be overridden to run code after all the test-methods have finished
         */
        virtual void tear_down()
        {
            //does nothing by default
        }

        /*!
         * Override this method to run code before every test-method
         * 
         * \return A boolean value. If false, the test-method will not be executed (inclusive \ref after)
         */
        virtual bool before(const std::string& methodName)
        {
            //does nothing by default
            return true;
        }

        /*!
         * Override this method to run code after every test-method
         */
        virtual void after(const std::string& methodName, const bool success)
        {
            //does nothing by default
        }

        static unsigned int totalTestMethods;

    private:

        struct TestMethod
        {
            const std::string name;
            const std::function<void(Suite*) > functor;
            const std::string argString;

            TestMethod() : name("")
            {
            }

            TestMethod(const std::string& name, SimpleTestMethod method) : name(name), functor(method), argString({})
            {
            }

            template<typename T>
            TestMethod(const std::string& name, SingleArgTestMethod<T> method, const T& arg) : 
                name(name), functor(std::bind(method, std::placeholders::_1, arg)), argString(std::to_string(arg))
            {
            }
            
            TestMethod(const std::string& name, SingleArgTestMethod<std::string> method, const std::string& arg) : 
                name(name), functor(std::bind(method, std::placeholders::_1, arg)), argString(std::string("\"")+arg+"\"")
            {
                //special case for std::string to enquote argument string
            }
            
            TestMethod(const std::string& name, SingleArgTestMethod<char*> method, char* arg) : 
                name(name), functor(std::bind(method, std::placeholders::_1, (char* const)arg)), argString((std::string("\"")+arg)+"\"")
            {
                //special case for char* to enquote argument string
            }
            
            template<typename... T>
            TestMethod(const std::string& name, VarargTestMethod<T...> method, const T... args) : name(name),
                functor([args..., method](Suite* suite) {(suite->*method)(args...);}), argString(joinStrings(args...))
            {
                    
            }
            
            inline void operator()(Suite* suite) const
            {
                functor(suite);
            }
            
            template<typename T, typename... R>
            static inline std::string joinStrings(const T& t, const R&... remainder)
            {
                if(sizeof...(R) == 0)
                    return std::to_string(t);
                return (std::to_string(t) + ", ") + joinStrings(remainder...);
            }
            
            template<typename... R>
            static inline std::string joinStrings(const std::string& t, const R&... remainder)
            {
                //special case to enquote std::string
                if(sizeof...(R) == 0)
                    return std::string("\"")+t+"\"";
                return (std::string("\"")+t+"\", ") + joinStrings(remainder...);
            }
            
            template<typename... R>
            static inline std::string joinStrings(const R&... remainder)
            {
                //is never called, but must exist to not throw compilation errors
                return "";
            }
        };
        std::string suiteName;
        std::vector<TestMethod> testMethods;
        std::vector<std::shared_ptr<Test::Suite>> subSuites;
        bool continueAfterFail;

        unsigned int positiveTestMethods;
        std::string currentTestMethodName;
        std::string currentTestMethodArgs;
        bool currentTestSucceeded;
        std::chrono::microseconds totalDuration;
        Output* output;

        std::pair<bool, std::chrono::microseconds> runTestMethod(const TestMethod& method);
        
        //ParallelSuite needs access to subSuites
        friend class ParallelSuite;
    };

    /*!
     * Registers a simple test-method
     */
#define TEST_ADD(func) setSuiteName(__FILE__); addTest(static_cast<SimpleTestMethod>(&func), #func)
    /*!
     * Registers a test-method taking a single argument of type std::string or a c-style string-literal
     */
#define TEST_ADD_WITH_STRING(func, string) setSuiteName(__FILE__); addTest(static_cast<SingleArgTestMethod<std::basic_string<char>>>(&func), #func, string)
    /*!
     * Registers a test-method taking a single argument of type c-string
     */
#define TEST_ADD_WITH_STRING_LITERAL(func, stringLiteral) setSuiteName(__FILE__); addTest(static_cast<SingleArgTestMethod<char*>>(&func), #func, stringLiteral)
    /*!
     * Registers a test-method accepting a single argument of type int (or any type which can be coerced from int)
     */
#define TEST_ADD_WITH_INTEGER(func, number) setSuiteName(__FILE__); addTest(static_cast<SingleArgTestMethod<int>>(&func), #func, number)
    /*!
     * Registers a test-method which takes an argument to a pointer of arbitrary data
     */
#define TEST_ADD_WITH_POINTER(func, pointer) setSuiteName(__FILE__); addTest(static_cast<SingleArgTestMethod<void*>>(&func), #func, pointer)
    /*!
     * Registers a test-method with a single argument of arbitrary type
     */
#define TEST_ADD_SINGLE_ARGUMENT(func, arg) setSuiteName(__FILE__); addTest(static_cast<SingleArgTestMethod<decltype(arg)>>(&func), #func, arg)
    /*!
     * Registers a test-method with two arguments of arbitrary types
     */
#define TEST_ADD_TWO_ARGUMENTS(func, arg0, arg1) setSuiteName(__FILE__); addTest<decltype(arg0), decltype(arg1)>(static_cast<VarargTestMethod<decltype(arg0), decltype(arg1)>>(&func), #func, arg0, arg1)
    /*!
     * Registers a test-method with three arguments of arbitrary types
     */
#define TEST_ADD_THREE_ARGUMENTS(func, arg0, arg1, arg2) setSuiteName(__FILE__); addTest<decltype(arg0), decltype(arg1), decltype(arg2)>(static_cast<VarargTestMethod<decltype(arg0), decltype(arg1), decltype(arg2)>>(&func), #func, arg0, arg1, arg2)
};

#endif	/* TESTSUITE_H */

