/* 
 * File:   TestFormat.cpp
 * Author: doe300
 * 
 * Created on March 4, 2017, 11:39 AM
 */

#include <cmath>

#include "TestFormat.h"

TestFormat::TestFormat()
{
    TEST_ADD(TestFormat::testFloatingPointNumbers);
    TEST_ADD(TestFormat::testStreamWriteOperator);
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

    std::ostream& operator<<(std::ostream& os) const
    {
        //do nothing
        return os;
    }

};

void TestFormat::testStreamWriteOperator()
{
    /* TODO
    ArbitraryType a;
    ArbitraryType b;
    TEST_ASSERT_EQUALS(a, b);
     */
}
