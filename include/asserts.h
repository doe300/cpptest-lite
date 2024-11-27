#pragma once

#include "comparisons.h"
#include "formatting.h"

#include <limits>

////
//  All asserts from cpptest-1.1.2
////

// TODO merge with modern functions and move most to (protected) TestSuite functions, in macros just set the __FILE__,
// __LINE__

#define TEST_FAIL(msg) testFailed(__FILE__, __LINE__, "", toMessage(msg));

#define TEST_ASSERT(condition)                                                                                         \
  testRun(static_cast<bool>(condition), __FILE__, __LINE__,                                                            \
      std::string("Assertion '") + #condition + std::string("' failed"), "");

#define TEST_ASSERT_MSG(condition, msg)                                                                                \
  testRun(static_cast<bool>(condition), __FILE__, __LINE__,                                                            \
      std::string("Assertion '") + #condition + std::string("' failed"), toMessage(msg));

#define TEST_ASSERT_EQUALS(expected, value)                                                                            \
  testRun(                                                                                                             \
      Test::Comparisons::isSame(expected, value), __FILE__, __LINE__,                                                  \
      [&] { return "Got " + Test::Formats::to_string(value) + ", expected " + Test::Formats::to_string(expected); },   \
      "");

#define TEST_ASSERT_EQUALS_MSG(expected, value, msg)                                                                   \
  testRun(                                                                                                             \
      Test::Comparisons::isSame(expected, value), __FILE__, __LINE__,                                                  \
      [&] { return "Got " + Test::Formats::to_string(value) + ", expected " + Test::Formats::to_string(expected); },   \
      toMessage(msg));

#define TEST_ASSERT_EQUALS_OBJ(expected, value)                                                                        \
  { static_assert(false, "This macro is deprecated, use TEST_ASSERT_EQUALS instead"); }
#define TEST_ASSERT_EQUALS_OBJ_MSG(expected, value, msg)                                                               \
  { static_assert(false, "This macro is deprecated, use TEST_ASSERT_EQUALS_MSG instead"); }
#define TEST_ASSERT_DELTA(expected, value, delta)                                                                      \
  testRun(                                                                                                             \
      Test::Comparisons::inMaxDistance(expected, value, delta), __FILE__, __LINE__,                                    \
      [&] {                                                                                                            \
        return "Got " + Test::Formats::to_string(value) + ", expected " + Test::Formats::to_string(expected) +         \
               " +/- " + Test::Formats::to_string(delta);                                                              \
      },                                                                                                               \
      "");

#define TEST_ASSERT_DELTA_MSG(expected, value, delta, msg)                                                             \
  testRun(                                                                                                             \
      Test::Comparisons::inMaxDistance(expected, value, delta), __FILE__, __LINE__,                                    \
      [&] {                                                                                                            \
        return "Got " + Test::Formats::to_string(value) + ", expected " + Test::Formats::to_string(expected) +         \
               " +/- " + Test::Formats::to_string(delta);                                                              \
      },                                                                                                               \
      toMessage(msg));

#define TEST_ASSERT_ULP(expected, value, numULP)                                                                       \
  {                                                                                                                    \
    using Type = std::remove_reference<decltype(expected)>::type;                                                      \
    auto delta = (expected) * static_cast<Type>(numULP) * std::numeric_limits<Type>::epsilon();                        \
    testRun(                                                                                                           \
        Test::Comparisons::inMaxDistance(expected, value, delta), __FILE__, __LINE__,                                  \
        [&] {                                                                                                          \
          return "Got " + Test::Formats::to_string(value) + ", expected " + Test::Formats::to_string(expected) +       \
                 " +/- " + (Test::Formats::to_string(delta) + " (") + (Test::Formats::to_string(numULP) + " ULP)");    \
        },                                                                                                             \
        "");                                                                                                           \
  }

#define TEST_ASSERT_ULP_MSG(expected, value, numULP, msg)                                                              \
  {                                                                                                                    \
    using Type = std::remove_reference<decltype(expected)>::type;                                                      \
    auto delta = (expected) * static_cast<Type>(numULP) * std::numeric_limits<Type>::epsilon();                        \
    testRun(                                                                                                           \
        Test::Comparisons::inMaxDistance(expected, value, delta), __FILE__, __LINE__,                                  \
        [&] {                                                                                                          \
          return "Got " + Test::Formats::to_string(value) + ", expected " + Test::Formats::to_string(expected) +       \
                 " +/- " + (Test::Formats::to_string(delta) + " (") + (Test::Formats::to_string(numULP) + " ULP)");    \
        },                                                                                                             \
        toMessage(msg));                                                                                               \
  }

#define TEST_THROWS(expression, except)                                                                                \
  {                                                                                                                    \
    try {                                                                                                              \
      expression;                                                                                                      \
      /*If we get here, no exception was thrown*/                                                                      \
      testFailed(__FILE__, __LINE__,                                                                                   \
          std::string("Expected exception of type '") + #except + std::string("' was not thrown!"), "");               \
    } catch (except &) {                                                                                               \
      testSucceeded(Test::Assertion(__FILE__, __LINE__));                                                              \
    } catch (std::exception & ex) {                                                                                    \
      /*If we get here, wrong exception was thrown*/                                                                   \
      testFailed(__FILE__, __LINE__, std::string("Wrong Exception was thrown: ") + ex.what(), "");                     \
    } catch (...) {                                                                                                    \
      /* Any other type than an exception was thrown*/                                                                 \
      testFailed(__FILE__, __LINE__,                                                                                   \
          std::string("A non-exception-type was thrown, expected exception of type: ") + #except, "");                 \
    }                                                                                                                  \
  }

#define TEST_THROWS_MSG(expression, except, msg)                                                                       \
  {                                                                                                                    \
    try {                                                                                                              \
      expression;                                                                                                      \
      /*If we get here, no exception was thrown*/                                                                      \
      testFailed(__FILE__, __LINE__,                                                                                   \
          std::string("Expected exception of type '") + #except + std::string("' was not thrown!"), toMessage(msg));   \
    } catch (except &) {                                                                                               \
      testSucceeded(Test::Assertion(__FILE__, __LINE__));                                                              \
    } catch (std::exception & ex) {                                                                                    \
      /*If we get here, wrong exception was thrown*/                                                                   \
      testFailed(__FILE__, __LINE__, std::string("Wrong Exception was thrown: ") + ex.what(), toMessage(msg));         \
    } catch (...) {                                                                                                    \
      /* Any other type than an exception was thrown*/                                                                 \
      testFailed(__FILE__, __LINE__,                                                                                   \
          std::string("A non-exception-type was thrown, expected exception of type: ") + #except, toMessage(msg));     \
    }                                                                                                                  \
  }

#define TEST_THROWS_ANYTHING(expression)                                                                               \
  {                                                                                                                    \
    try {                                                                                                              \
      expression;                                                                                                      \
      /*If we get here, no exception was thrown*/                                                                      \
      testFailed(__FILE__, __LINE__, "Expected exception, nothing was thrown!", "");                                   \
    } catch (std::exception &) {                                                                                       \
      testSucceeded(Test::Assertion(__FILE__, __LINE__));                                                              \
    } catch (...) {                                                                                                    \
      /* Any other type than an exception was thrown*/                                                                 \
      testFailed(__FILE__, __LINE__, "A non-exception-type was thrown!", "");                                          \
    }                                                                                                                  \
  }

#define TEST_THROWS_ANYTHING_MSG(expression, msg)                                                                      \
  {                                                                                                                    \
    try {                                                                                                              \
      expression;                                                                                                      \
      /*If we get here, no exception was thrown*/                                                                      \
      testFailed(__FILE__, __LINE__, "Expected exception, nothing was thrown!", toMessage(msg));                       \
    } catch (std::exception &) {                                                                                       \
      testSucceeded(Test::Assertion(__FILE__, __LINE__));                                                              \
    } catch (...) {                                                                                                    \
      /* Any other type than an exception was thrown*/                                                                 \
      testFailed(__FILE__, __LINE__, "A non-exception-type was thrown!", toMessage(msg));                              \
    }                                                                                                                  \
  }

#define TEST_THROWS_NOTHING(expression)                                                                                \
  {                                                                                                                    \
    try {                                                                                                              \
      expression;                                                                                                      \
      /*If we get here, no exception was thrown*/                                                                      \
      testSucceeded(Test::Assertion(__FILE__, __LINE__));                                                              \
    } catch (std::exception & ex) {                                                                                    \
      testFailed(__FILE__, __LINE__, std::string("Exception thrown: ") + ex.what(), "");                               \
    } catch (...) {                                                                                                    \
      /* Any other type than an exception was thrown*/                                                                 \
      testFailed(__FILE__, __LINE__, "A non-exception-type was thrown!", "");                                          \
    }                                                                                                                  \
  }

#define TEST_THROWS_NOTHING_MSG(expression, msg)                                                                       \
  {                                                                                                                    \
    try {                                                                                                              \
      expression;                                                                                                      \
      /*If we get here, no exception was thrown*/                                                                      \
      testSucceeded(Test::Assertion(__FILE__, __LINE__));                                                              \
    } catch (std::exception & ex) {                                                                                    \
      testFailed(__FILE__, __LINE__, std::string("Exception thrown: ") + ex.what(), toMessage(msg));                   \
    } catch (...) {                                                                                                    \
      /* Any other type than an exception was thrown*/                                                                 \
      testFailed(__FILE__, __LINE__, "A non-exception-type was thrown!", toMessage(msg));                              \
    }                                                                                                                  \
  }

////
//  Custom asserts
////
#define TEST_PREDICATE(predicate, value)                                                                               \
  testRun(                                                                                                             \
      predicate(value), __FILE__, __LINE__,                                                                            \
      [&] { return "Value '" + Test::Formats::to_string(value) + "' did not match the predicate: " + #predicate; },    \
      "");

#define TEST_PREDICATE_MSG(predicate, value, msg)                                                                      \
  testRun(                                                                                                             \
      predicate(value), __FILE__, __LINE__,                                                                            \
      [&] { return "Value '" + Test::Formats::to_string(value) + "' did not match the predicate: " + #predicate; },    \
      toMessage(msg));

#define TEST_BIPREDICATE(bipredicate, value0, value1)                                                                  \
  testRun(                                                                                                             \
      bipredicate(value0, value1), __FILE__, __LINE__,                                                                 \
      [&] {                                                                                                            \
        return "Values '" + Test::Formats::to_string(value0) + "' and '" + Test::Formats::to_string(value1) +          \
               "' did not match the bi-predicate: " + #bipredicate;                                                    \
      },                                                                                                               \
      "");

#define TEST_BIPREDICATE_MSG(bipredicate, value0, value1, msg)                                                         \
  testRun(                                                                                                             \
      bipredicate(value0, value1), __FILE__, __LINE__,                                                                 \
      [&] {                                                                                                            \
        return "Values '" + Test::Formats::to_string(value0) + "' and '" + Test::Formats::to_string(value1) +          \
               "' did not match the bi-predicate: " + #bipredicate;                                                    \
      },                                                                                                               \
      toMessage(msg));

#define TEST_ABORT(msg)                                                                                                \
  {                                                                                                                    \
    testFailed(Test::Assertion(__FILE__, __LINE__, "Test-method aborted!", toMessage(msg)));                           \
    return;                                                                                                            \
  }

#define TEST_STRING_EQUALS(expected, value)                                                                            \
  testRun(                                                                                                             \
      std::string(expected) == (value), __FILE__, __LINE__,                                                            \
      [&] { return "Got \"" + std::string(value) + "\", expected \"" + std::string(expected) + "\""; }, "");

#define TEST_STRING_EQUALS_MSG(expected, value, msg)                                                                   \
  testRun(                                                                                                             \
      std::string(expected) == (value), __FILE__, __LINE__,                                                            \
      [&] { return "Got \"" + std::string(value) + "\", expected \"" + std::string(expected) + "\""; },                \
      toMessage(msg));

#define TEST_ASSERT_NOT_EQUALS(expected, value)                                                                        \
  testRun(                                                                                                             \
      !Test::Comparisons::isSame(expected, value), __FILE__, __LINE__,                                                 \
      [&] {                                                                                                            \
        return "Expected " + Test::Formats::to_string(value) + " to differ from " +                                    \
               Test::Formats::to_string(expected);                                                                     \
      },                                                                                                               \
      "");

#define TEST_ASSERT_NOT_EQUALS_MSG(expected, value, msg)                                                               \
  testRun(                                                                                                             \
      !Test::Comparisons::isSame(expected, value), __FILE__, __LINE__,                                                 \
      [&] {                                                                                                            \
        return "Expected " + Test::Formats::to_string(value) + " to differ from " +                                    \
               Test::Formats::to_string(expected);                                                                     \
      },                                                                                                               \
      toMessage(msg));

#define TEST_ASSERT_FALSE(condition)                                                                                   \
  testRun(!static_cast<bool>(condition), __FILE__, __LINE__,                                                           \
      std::string("Assertion '") + #condition + std::string("' passed unexpectedly"), "");

#define TEST_ASSERT_FALSE_MSG(condition, msg)                                                                          \
  testRun(!static_cast<bool>(condition), __FILE__, __LINE__,                                                           \
      std::string("Assertion '") + #condition + std::string("' passed unexpectedly"), toMessage(msg));
