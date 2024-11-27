#include <iostream>
#include <optional>
#include <variant>
#include <vector>

#include "../include/cpptest-main.h"

using namespace std;

#if defined(__cpp_lib_source_location) && defined(__cpp_lib_string_view)
struct NonCopyOrMoveableType {
  NonCopyOrMoveableType(int val) : value(val) {}
  NonCopyOrMoveableType(const NonCopyOrMoveableType &) = delete;
  NonCopyOrMoveableType(NonCopyOrMoveableType &&) noexcept = delete;
  ~NonCopyOrMoveableType() noexcept = default;

  NonCopyOrMoveableType &operator=(const NonCopyOrMoveableType &) = delete;
  NonCopyOrMoveableType &operator=(NonCopyOrMoveableType &&) noexcept = delete;

  bool operator==(const NonCopyOrMoveableType &other) const noexcept { return value == other.value; }
  explicit operator int() const noexcept { return value; }

  friend std::ostream &operator<<(std::ostream &os, const NonCopyOrMoveableType &val) { return os << val.value; }

  int value;
};

struct OneComparableType {
  friend std::ostream &operator<<(std::ostream &os, const OneComparableType &) { return os << "one"; }
};

struct OtherComparableType {
  friend bool operator==(const OneComparableType &, const OtherComparableType &) noexcept { return true; }
  friend std::ostream &operator<<(std::ostream &os, const OtherComparableType &) { return os << "other"; }
};

struct TestModernAssertions : public Test::Suite {
  TestModernAssertions() : Test::Suite("ModernAssertions") {
    TEST_ADD(TestModernAssertions::testAssertions);
    TEST_ADD(TestModernAssertions::testTypeSupport);
  }

  void testAssertions() {
    testFail("Foo");
    testFail(std::string{"Some"} + " dynamic" + " text");
    testAssert(1 == 3);
    testAssert(2 == 3, "Static text");
    testAssert(4 == 3, std::string{"Some"} + " dynamic" + " text");
    testAssert([]() { return false; });
    testAssertEquals(3, 17);
    NonCopyOrMoveableType a{1};
    NonCopyOrMoveableType b{13};
    testAssertEquals(a, b, "Custom text");
    testAssertEquals(32, b);
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

    OneComparableType one{};
    OtherComparableType other{};
    testAssertEquals(one, other);
  }

  void testTypeSupport() {
    std::string s1 = "Goo";
    std::string_view s2 = "Bar";
    testAssertEquals(s1, s2);

    std::optional<float> o1{-17.3f};
    std::optional<float> o2{};
    testAssertEquals(o1, o2);
    testAssertEquals(true, o2);
    testAssertEquals(o1, false);

    std::vector<int> c1{1, 2, 3, 4, 5};
    std::vector<int> c2{7, 6, 5};
    std::span<int> c3(c1.begin(), c1.end());
    std::span<int> c4(c2.begin(), c2.end());
    testAssertEquals(c3, c4);
    testAssertEquals(c3, c3);

    std::variant<double, std::string> v1{9.0};
    std::variant<double, std::string> v2{"Foo"};
    testAssertEquals(v1, v2);

    std::array<uint8_t, 3> span0{0x00, 0x01, 0x13};
    std::vector<uint8_t> span1{0x01, 0x02, 0x03};
    testAssertEquals(span0, span1);

#if defined(CPPTEST_LITE_U8_STRING)
    std::u8string u1{u8"Foo Bar"};
    std::u8string u2{u8"⠁⠂⠃"};
    std::u8string_view u3 = u8"Baz";
    testAssertEquals(u1, u2);
    testAssertEquals(u1, u3);
    testAssertEquals(u1.c_str(), u3.data());
#endif

    std::u16string w1{u"Foo Bar"};
    std::u16string w2{u"\u2801⠐⠂⠃"};
    std::u16string_view w3 = u"Baz";
    testAssertEquals(w1, w2);
    testAssertEquals(w1, w3);
    testAssertEquals(w1.c_str(), w3.data());

    std::u32string utf1{U"Foo Bar"};
    std::u32string utf2{U"\u2801⠐⠂⠃"};
    std::u32string_view utf3 = U"Baz";
    testAssertEquals(utf1, utf2);
    testAssertEquals(utf1, utf3);
    testAssertEquals(utf1.c_str(), utf3.data());
  }
};

// Main test program
//
int main(int argc, char *argv[]) {
  Test::setContinueAfterFail(true);
  Test::registerSuite(
      Test::newInstance<TestModernAssertions>, "modern", "Test workings of modern (non-macro) assertions");
  return Test::runSuites(argc, argv);
}
#else
int main(int argc, char *argv[]) {
  // Not supported
  return EXIT_FAILURE;
}
#endif
