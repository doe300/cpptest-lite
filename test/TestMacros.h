/* 
 * File:   TestMacros.h
 * Author: daniel
 *
 * Created on September 15, 2015, 6:27 PM
 */

#ifndef TESTMACROS_H
#define	TESTMACROS_H

#include "cpptest.h"

class TestMacros : public Test::Suite
{
public:
    TestMacros();
    
    void testMethodWithStringArg(char* arg);
    
    void testMethodWithIntArg(const int arg);
    
    void testMethodWithPointerArg(void* arg);
    
};

#endif	/* TESTMACROS_H */

