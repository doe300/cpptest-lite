#pragma once

#include <cmath>

#include "cpptest-bdd.h"

STORY(Story1)

SCENARIO(Scenario1)
GIVEN(__cplusplus >= 201103L) // if C++11 is supported
WHEN(TEST_ASSERT_EQUALS(2.0, sqrt(4.0)))
THEN(true)
END_SCENARIO

SCENARIO(ShouldFail)
GIVEN(true)
WHEN()
THEN(false)
END_SCENARIO

SCENARIO(FailsToo)
GIVEN(false)
WHEN(TEST_ASSERT_EQUALS(1, 1))
THEN(true)
END_SCENARIO

SCENARIO(LastFail)
GIVEN(true)
WHEN(TEST_ASSERT_EQUALS(2, 1))
THEN(true)
END_SCENARIO

SCENARIO(ComplexScenario)
GIVEN(sizeof(char) == 1 && sizeof(short) == 2 && sizeof(int) == 4)
WHEN(TEST_ASSERT_EQUALS(static_cast<short>(1), static_cast<int>(1)); TEST_ASSERT(sizeof(short) < sizeof(int));
     TEST_ASSERT(true != false))
THEN(true)
END_SCENARIO

END_STORY

STORY_WITH_MEMBERS(Story2, int a; int b)
// Empty story
END_STORY

STORY_WITH_MEMBERS(Story3, int a; int b)

SCENARIO(TestMembers)
GIVEN((a = 5, b = 3, true))
WHEN(TEST_ASSERT(5 > b); TEST_ASSERT_EQUALS(3, b))
THEN(true)
END_SCENARIO

END_STORY
