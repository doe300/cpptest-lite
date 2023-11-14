/*
* File:   run_modern_tests.h
* Author: doe300
*
* Created on October 29, 2023
*/

#include <iostream>
#include <optional>
#include <variant>
#include <vector>

#include "../include/cpptest-main.h"

using namespace std;

struct NonCopyOrMoveableType
{
    NonCopyOrMoveableType(int val) : value(val) {}
    NonCopyOrMoveableType(const NonCopyOrMoveableType&) = delete;
    NonCopyOrMoveableType(NonCopyOrMoveableType&&) noexcept = delete;
    ~NonCopyOrMoveableType() noexcept = default;

    NonCopyOrMoveableType& operator=(const NonCopyOrMoveableType&) = delete;
    NonCopyOrMoveableType& operator=(NonCopyOrMoveableType&&) noexcept = delete;

    bool operator==(const NonCopyOrMoveableType& other) const noexcept
    {
        return value == other.value;
    }

    friend std::ostream& operator<<(std::ostream& os, const NonCopyOrMoveableType& val)
    {
        return os << val.value;
    }

    int value;
};

struct TestModernAssertions : public Test::Suite
{
    TestModernAssertions() : Test::Suite("ModernAssertions")
    {
        TEST_ADD(TestModernAssertions::testAssertions);
        TEST_ADD(TestModernAssertions::testTypeSupport);
    }

    void testAssertions()
    {
        testFail("Foo");
        testFail(std::string{"Some"} + " dynamic" + " text");
        testAssert(1 == 3);
        testAssert(2 == 3, "Static text");
        testAssert(4 == 3, std::string{"Some"} + " dynamic" + " text");
        testAssert([] () { return false; });
        testAssertEquals(3, 17);
        NonCopyOrMoveableType a{1};
        NonCopyOrMoveableType b{13};
        testAssertEquals(a, b, "Custom text");
        testAssertDelta(3, 17, 3, std::string{"Some range check"});
        testAssertUlp(3.0, 17.0, 3.0, "Of course this fails!");
        testThrows<std::invalid_argument>([] { return 3; });
        testThrows<std::runtime_error>([] { throw 1; }, "Not a runtime error");
        testThrowsAnything([] {});
        testThrowsAnything([] { throw 17; }, "This is not an exception");
        testThrowsNothing([] { throw std::runtime_error{"Foo"}; });
        testThrowsNothing([] { throw 42; }, "Another non-exception");
        testPredicate(std::identity{}, false);
        testBiPredicate(std::equal_to<>{}, a, b);
        testAbort("This is the end!");
    }

    void testTypeSupport()
    {
        std::string s1 = "Goo";
        std::string_view s2 = "Bar";
        testAssertEquals(s1, s2);

        std::optional<float> o1{-17.3f};
        std::optional<float> o2{};
        testAssertEquals(o1, o2);

        std::vector<int> c1{1, 2, 3, 4, 5};
        std::vector<int> c2{7, 6, 5};
        std::span<int> c3(c1.begin(), c1.end());
        std::span<int> c4(c2.begin(), c2.end());
        testAssertEquals(c3, c4);
        testAssertEquals(c3, c3);

        std::variant<double, std::string> v1{9.0};
        std::variant<double, std::string> v2{"Foo"};
        testAssertEquals(v1, v2);
    }
};

// Main test program
//
int main(int argc, char* argv[])
{
    Test::setContinueAfterFail(true);
    Test::registerSuite(Test::newInstance<TestModernAssertions>, "modern", "Test workings of modern (non-macro) assertions");
    return Test::runSuites(argc, argv);
}
