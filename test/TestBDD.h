/* 
 * File:   TestBDD.h
 * Author: doe300
 *
 * Created on June 29, 2016, 3:17 PM
 */

#ifndef TESTBDD_H
#define TESTBDD_H

#include <cmath>

#include "cpptest-bdd.h"

STORY(Story1)
        
    SCENARIO(Scenario1)
    GIVEN(__cplusplus >= 201103L)       //if C++11 is supported
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
    WHEN(TEST_ASSERT_EQUALS((short)1, (int)1); TEST_ASSERT(sizeof(short) < sizeof(int)); TEST_ASSERT(true != false))
    THEN(true)
    END_SCENARIO
        
END_STORY
            
STORY(Story2)
            //Empty story
END_STORY            

#endif /* TESTBDD_H */

