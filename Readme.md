# CppTest-lite

[![Build Status](https://travis-ci.org/doe300/cpptest-lite.svg)](https://travis-ci.org/doe300/cpptest-lite)
[![Build Status](https://ci.appveyor.com/api/projects/status/xoa5e8eiwhilifvq?svg=true)](https://ci.appveyor.com/project/doe300/cpptest-lite)
[![GitHub license](https://img.shields.io/github/license/doe300/cpptest-lite.svg)](https://github.com/doe300/cpptest-lite/blob/master/LICENSE)
[![Release](https://img.shields.io/github/tag/doe300/cpptest-lite.svg)](https://github.com/doe300/cpptest-lite/releases/latest)

This is an approximate re-implementation of the [cpptest testing framework](http://sourceforge.net/projects/cpptest/).
This library is based on the version *1.1.2*.

The original cpptest-framework is not longer in active development and I personally found it too complex for the simple tasks it runs.
So I implemented this smaller version in initially around 2 days of work.

CppTest-lite is a lot smaller in size (<200KB compared to >2MB) and runs tests slightly faster.

## Compatibility
Runs the test-program of the original *cpptest-1.1.2*.

Some minor changes were made to the behavior:

- the macros **TEST_ASSERT_EQUALS_OBJ** and **TEST_ASSERT_EQUALS_OBJ_MSG** were disabled (create compiler-error), because they 
do the exact same thing as **TEST_ASSERT_EQUALS** and **TEST_ASSERT_EQUALS_MSG**
- the macros **TEST_THROWS_ANYTHING** and **TEST_THROWS_ANYTHING_MSG** treat throwing any type not extending std::exception as failure, 
because - seriously - that is bad coding style
- the **add(auto_ptr&lt;Suite&gt;)** method was rewritten to use the new **shared_ptr**.
- **Test::Suite.setup** now returns a **bool** value determining whether to continue running the test-suite
- Split user-message and failure-message into two separate fields (two separate lines in *TextOutput*)

## New Features (latest version)
- based upon the (new) C++11 standard
- new macros **TEST_PREDICATE(_MSG)** and **TEST_BIPREDICATE(_MSG)** for testing a single (or two) values with a predicate.
Additionally, two new types were created: **Test::Predicate** and **Test::BiPredicate**, but basically any method accepting a single (or two) arguments and 
returning a *bool* or any other type which can be coerced into a *bool* can be used.
- supports parameterized test-methods and multiple registrations for same test-method. The parameters are automatically passed to the test-method and the resulting output
- added macro **TEST_ABORT** to fail and abort a test-method
- added methods to be executed **before** and **after** every test-method. **setup** and **before** can be used to skip the suite/a single test-method.
- More detailed failure-message, e.g. prints the failed assertion and the method-parameters, if there are any
- **ParallelSuite** allows to run TestSuites in a multi-threaded environment and automatically synchronizes access to the Output.
- new macros to add tests, **TEST_ADD_SINGLE_ARGUMENT**, **TEST_ADD_TWO_ARGUMENTS** and **TEST_ADD_TWO_ARGUMENTS** 
to add test with one, two or three arguments of arbitrary types.
- new macros to compare string-equality **TEST_STRING_EQUALS** and **TEST_STRING_EQUALS_MSG** supporting *std::string* and *string-constants*.
- **TEST_ASSERT_EQUALS(_MSG)** now also supports string equality as well as comparison of *NaNs*
- Help-page and selection of test-suites to run can be automatically generated via *Test::registerSuite()* and *Test::runSuites*

### Behavior driven development
- As of version 0.6 BDD is supported as a completely new feature
- write stories and scenarios easily in a text-based style (see *TestBDD.h* for a few examples)
- macro-preprocessor automatically generates a **Suite** for every given Story