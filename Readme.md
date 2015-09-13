# CppTest-lite
This is an approximate re-implementation of the [cpptest testing framework](http://sourceforge.net/projects/cpptest/).
This library is based on the version *1.1.2*.

The original cpptest-framework is not longer in active development and I personally found it too complex for the simple tasks it runs.
So I implement this smaller version in around 2 days of work.

CppTest-lite is a lot smaller in size (<100KB compared to >2MB) and runs tests slightly faster.

## Compatibility
Runs the test-program of the original *cpptest-1.1.2*.

Some minor changes were made to the behavior:

- the macros **TEST_ASSERT_EQUALS_OBJ** and **TEST_ASSERT_EQUALS_OBJ_MSG** were disabled (create compiler-error), because they 
do the exact same thing as **TEST_ASSERT_EQUALS** and **TEST_ASSERT_EQUALS_MSG**
- the macros **TEST_THROWS_ANYTHING** and **TEST_THROWS_ANYTHING_MSG** treat throwing any type not extending std::exception as failure, 
because - seriously - that is bad coding style
- the **add(auto_ptr&lt;Suite&gt;)** method was rewritten to use the new **shared_ptr**.
- **Test::CollectorOutput**, **Test::CompilerOutput** and **Test::HTMLOutput** are not yet available

## New Features
- based upon the (new) C++11 standard
- new macros **TEST_PREDICATE(_MSG)** and **TEST_BIPREDICATE(_MSG)** for testing a single (or two) values with a predicate.
Additionally, two new types were created: **Test::Predicate** and **Test::BiPredicate**, but basically any method accepting a single (or two) arguments and 
returning a *bool* or any other type which can be coerced into a *bool* can be used.
- supports parameterized test-methods and multiple registrations for same test-method. Currently **TEST_ADD_WITH_STRING** allows for a string-parameter to be passed,
while **TEST_ADD_WITH_INTEGER** accepts an integer and **TEST_ADD_WITH_POINTER** an arbitrary pointer. These values are automatically passed to the test-method
- added macro **TEST_ABORT** to fail and abort a test-method
- added methods to be executed **before** and **after** every test-method