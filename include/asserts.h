/* 
 * File:   asserts.h
 * Author: daniel
 *
 * Created on September 10, 2015, 11:22 AM
 */

#ifndef ASSERTS_H
#define	ASSERTS_H

#include "comparisons.h"
#include "formatting.h"

#include <limits>

////
//  All asserts from cpptest-1.1.2
////
    
    
#define TEST_FAIL(msg) \
    { \
        testFailed(Test::Assertion(__FILE__, __LINE__, ((msg) != nullptr ? #msg : ""))); \
        if(!continueAfterFailure()) return; \
    }
    
#define TEST_ASSERT(condition) \
    { \
        if(false == (condition)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Assertion '") + #condition + std::string("' failed"), "")); \
            if(!continueAfterFailure()) return; \
        } \
        else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }

#define TEST_ASSERT_MSG(condition, msg) \
    { \
        if(false == (condition)) { \
            testFailed(Test::Assertion( __FILE__, __LINE__, std::string("Assertion '") + #condition + std::string("' failed"), ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
        else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
    
#define TEST_ASSERT_EQUALS(expected, value) \
    { \
        if(!Test::Comparisons::isSame(expected, value)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Got ") + Test::Formats::to_string(value) + std::string(", expected ") + Test::Formats::to_string(expected), "")); \
            if(!continueAfterFailure()) return; \
        } \
        else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
#define TEST_ASSERT_EQUALS_MSG(expected, value, msg) \
    { \
        if(!Test::Comparisons::isSame(expected, value)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Got ") + Test::Formats::to_string(value) + std::string(", expected ") + Test::Formats::to_string(expected), ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
        else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
#define TEST_ASSERT_EQUALS_OBJ(expected, value) \
    { \
        static_assert(false, "This macro is deprecated, use TEST_ASSERT_EQUALS instead"); \
    }
#define TEST_ASSERT_EQUALS_OBJ_MSG(expected, value, msg) \
    { \
        static_assert(false, "This macro is deprecated, use TEST_ASSERT_EQUALS_MSG instead"); \
    }
#define TEST_ASSERT_DELTA(expected, value, delta) \
    { \
        if(!Test::Comparisons::inMaxDistance(expected, value, delta)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Got ") + Test::Formats::to_string(value) + std::string(", expected ") + Test::Formats::to_string(expected) + std::string(" +/- ") + Test::Formats::to_string(delta), "")); \
            if(!continueAfterFailure()) return; \
        } \
        else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
#define TEST_ASSERT_DELTA_MSG(expected, value, delta, msg) \
    { \
        if(!Test::Comparisons::inMaxDistance(expected, value, delta)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Got ") + Test::Formats::to_string(value) + std::string(", expected ") + Test::Formats::to_string(expected) + std::string(" +/- ") + Test::Formats::to_string(delta), ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
        else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
#define TEST_ASSERT_ULP(expected, value, numULP) \
    { \
		using Type = std::remove_reference<decltype(expected)>::type; \
		auto delta = (expected) * static_cast<Type>(numULP) * std::numeric_limits<Type>::epsilon(); \
        if(!Test::Comparisons::inMaxDistance(expected, value, delta)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Got ") + Test::Formats::to_string(value) + std::string(", expected ") + Test::Formats::to_string(expected) + std::string(" +/- ") + (Test::Formats::to_string(delta) + " (") + (Test::Formats::to_string(numULP) + " ULP)"), "")); \
            if(!continueAfterFailure()) return; \
        } \
        else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
#define TEST_ASSERT_ULP_MSG(expected, value, numULP, msg) \
    { \
		using Type = std::remove_reference<decltype(expected)>::type; \
		auto delta = (expected) * static_cast<Type>(numULP) * std::numeric_limits<Type>::epsilon(); \
        if(!Test::Comparisons::inMaxDistance(expected, value, delta)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Got ") + Test::Formats::to_string(value) + std::string(", expected ") + Test::Formats::to_string(expected) + std::string(" +/- ") + (Test::Formats::to_string(delta) + " (") + (Test::Formats::to_string(numULP) + " ULP)"), ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
        else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
#define TEST_THROWS(expression, except) \
    { \
        try { \
            expression; \
            /*If we get here, no exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__,__LINE__, std::string("Expected exception of type '") + #except + std::string("' was not thrown!"), "")); \
            if(!continueAfterFailure()) return; \
        } \
        catch(except&) { testSucceeded(Test::Assertion(__FILE__,__LINE__)); } \
        catch(std::exception &ex) { \
            /*If we get here, wrong exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Wrong Exception was thrown: ") + ex.what(), "")); \
            if(!continueAfterFailure()) return; \
        } \
        catch(...) { \
            /* Any other type than an exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("A non-exception-type was thrown, expected exception of type: ") + #except, "")); \
            if(!continueAfterFailure()) return; \
        } \
    }
#define TEST_THROWS_MSG(expression, except, msg) \
    { \
        try { \
            expression; \
            /*If we get here, no exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Expected exception of type '") + #except + std::string("' was not thrown!"), ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
        catch(except&) { testSucceeded(Test::Assertion(__FILE__,__LINE__)); } \
        catch(std::exception &ex) { \
            /*If we get here, wrong exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Wrong Exception was thrown: ") + ex.what(), ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
        catch(...) { \
            /* Any other type than an exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("A non-exception-type was thrown, expected exception of type: ") + #except, ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
    }
#define TEST_THROWS_ANYTHING(expression) \
    { \
        try { \
            expression; \
            /*If we get here, no exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__, __LINE__, "Expected exception, nothing was thrown!", "")); \
            if(!continueAfterFailure()) return; \
        } \
        catch(std::exception &ex) { \
            testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
        } \
        catch(...) { \
            /* Any other type than an exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__, __LINE__, "A non-exception-type was thrown!", "")); \
            if(!continueAfterFailure()) return; \
        } \
    }
#define TEST_THROWS_ANYTHING_MSG(expression, msg) \
    { \
        try { \
            expression; \
            /*If we get here, no exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__, __LINE__, "Expected exception, nothing was thrown!", ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
        catch(std::exception &ex) { \
            testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
        } \
        catch(...) { \
            /* Any other type than an exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__, __LINE__, "A non-exception-type was thrown!", ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
    }
#define TEST_THROWS_NOTHING(expression) \
    { \
        try { \
            expression; \
            /*If we get here, no exception was thrown*/ \
            testSucceeded(Test::Assertion(__FILE__, __LINE__)); \
        } \
        catch(std::exception &ex) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Exception thrown: ") + ex.what(), "")); \
            if(!continueAfterFailure()) return; \
        } \
        catch(...) { \
            /* Any other type than an exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__, __LINE__, "A non-exception-type was thrown!", "")); \
            if(!continueAfterFailure()) return; \
        } \
    }
#define TEST_THROWS_NOTHING_MSG(expression, msg) \
    { \
        try { \
            expression; \
            /*If we get here, no exception was thrown*/ \
            testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
        } \
        catch(std::exception &ex) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Exception thrown: ") + ex.what(), ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
        catch(...) { \
            /* Any other type than an exception was thrown*/ \
            testFailed(Test::Assertion(__FILE__, __LINE__, "A non-exception-type was thrown!", ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
    }

////
//  Custom asserts
////
#define TEST_PREDICATE(predicate, value) \
    { \
        if(false == predicate(value)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Value '") + Test::Formats::to_string(value) + std::string("' did not match the predicate: ") + #predicate, "")); \
            if(!continueAfterFailure()) return; \
        } else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }   
#define TEST_PREDICATE_MSG(predicate, value, msg) \
    { \
        if(false == predicate(value)) {\
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Value '") + Test::Formats::to_string(value) + std::string("' did not match the predicate: ") + #predicate, ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
#define TEST_BIPREDICATE(bipredicate, value0, value1) \
    { \
        if(false == bipredicate(value0, value1)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Values '") + Test::Formats::to_string(value0) + "' and '" + Test::Formats::to_string(value1) + std::string("' did not match the bi-predicate: ") + #bipredicate, "")); \
            if(!continueAfterFailure()) return; \
        } else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
#define TEST_BIPREDICATE_MSG(bipredicate, value0, value1, msg) \
    { \
        if(false == bipredicate(value0, value1)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Values '") + Test::Formats::to_string(value0) + "' and '" + Test::Formats::to_string(value1) + std::string("' did not match the bi-predicate: ") + #bipredicate, ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
#define TEST_ABORT(msg) \
    { \
        testFailed(Test::Assertion(__FILE__, __LINE__, "Test-method aborted!", ((msg) != nullptr ? #msg : ""))); \
        return; \
    }
#define TEST_STRING_EQUALS(expected, value) \
    { \
        if(std::string(expected) != (value)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Got \"") + std::string(value) + std::string("\", expected \"") + std::string(expected) + std::string("\""), "")); \
            if(!continueAfterFailure()) return; \
        } \
        else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
#define TEST_STRING_EQUALS_MSG(expected, value, msg) \
    { \
        if(std::string(expected) != (value)) { \
            testFailed(Test::Assertion(__FILE__, __LINE__, std::string("Got \"") + std::string(value) + std::string("\", expected \"") + std::string(expected) + std::string("\""), ((msg) != nullptr ? #msg : ""))); \
            if(!continueAfterFailure()) return; \
        } \
        else testSucceeded(Test::Assertion(__FILE__,__LINE__)); \
    }
#endif	/* ASSERTS_H */

