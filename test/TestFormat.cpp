#include <cmath>
#include <cstdint>
#include <tuple>
#include <vector>

#include "TestFormat.h"

TestFormat::TestFormat()
{
    TEST_ADD(TestFormat::testFloatingPointNumbers);
    TEST_ADD(TestFormat::testStreamWriteOperator);
    TEST_ADD(TestFormat::testStringTypes);
    TEST_ADD(TestFormat::testMemoryRanges);
    TEST_ADD(TestFormat::testTuples);
    TEST_ADD(TestFormat::testNonLiteralMessages);
}

void TestFormat::testFloatingPointNumbers()
{
    TEST_ASSERT_EQUALS_MSG(0.0f, std::nextafter(0.0f, 1.0f), "Should fail with two different numbers");
}

struct ArbitraryType
{

    bool operator==(const ArbitraryType& right) const
    {
        return this == &right;
    }

    bool operator!=(const ArbitraryType& right) const
    {
        bool result = !(*this == right);
        return result;
    }

    std::string toString() const
    {
    	return std::string("toString(") + std::to_string(reinterpret_cast<uintptr_t>(this)) + ")";
    }

    std::string to_string() const
    {
    	return std::string("to_string(") + std::to_string(reinterpret_cast<uintptr_t>(this)) + ")";
    }
};

std::ostream& operator<<(std::ostream& os, const ArbitraryType& val)
{
	os << "stream-operator(" << &val << ")";
	return os;
}

void TestFormat::testStreamWriteOperator()
{
    ArbitraryType a;
    ArbitraryType b;
    TEST_ASSERT_EQUALS(a, b);
}

void TestFormat::testStringTypes()
{
    TEST_ASSERT_EQUALS_MSG(u"†‡©®™", static_cast<const char16_t*>(nullptr), "Should be printed correctly");
    TEST_ASSERT_EQUALS_MSG(std::u16string{u"⠁⠂⠄⠈⠐⠠⡀"}, u"†‡©®™", "Should be printed correctly");
    TEST_ASSERT_EQUALS_MSG(std::u32string{U"🚸⛔🔞"}, U"🔙🔚🔛🔜🔝", "Should be printed correctly");
}

void TestFormat::testMemoryRanges()
{
    const std::vector<uint32_t> A = {1, 2, 3, 4, 5};
    const std::vector<uint32_t> B = {7, 5, 3, 1};
    TEST_ASSERT_EQUALS_MSG(A, B, "Vector elements should be printed");
}

void TestFormat::testTuples()
{
    auto a = std::make_tuple(7, 8U, 3.5, 7.1f, true);
    std::tuple<int, unsigned, double, float, bool> b{13, 4U, 2.7, 1.9F, false};
    TEST_ASSERT_EQUALS_MSG(a, b, "Tuple elements should be printed");
}

void TestFormat::testNonLiteralMessages()
{
    // checks compatibility with old string literal only calls which handle NULL pointer
    TEST_ASSERT_MSG(false, nullptr);
    for(int i = 0; i < 2; ++i)
    {
        TEST_ASSERT_EQUALS_MSG(i, i - 1, std::to_string(i) + " and " + std::to_string(i - 1) + " are never equal");
    }
}
