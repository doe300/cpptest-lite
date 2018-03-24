// ---
//
// $Id: mytest.cpp,v 1.5 2008/07/11 16:49:43 hartwork Exp $
//
// CppTest - A C++ Unit Testing Framework
// Copyright (c) 2003 Niklas Lundell
//
// ---
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// ---
//
// Test program demonstrating all assert types and output handlers.
//
// ---

#include <iostream>

#include "../include/cpptest-main.h"
#include "TestMacros.h"
#include "TestOutputs.h"
#include "TestParallelSuite.h"
#include "TestBDD.h"
#include "TestFormat.h"
#include "TestAssertions.h"

using namespace std;

// Main test program
//
int main(int argc, char* argv[])
{
    Test::setContinueAfterFail(true);
    Test::registerSuite(Test::newInstance<FailTestSuite>, "fail-tests", "Tests the correct handling of failed tests");
    Test::registerSuite(Test::newInstance<CompareTestSuite>, "test-comparisons", "Tests the comparison tests");
    Test::registerSuite(Test::newInstance<ThrowTestSuite>, "test-exceptions", "Tests correct handling of exceptions (expected and unexpected)");
    Test::registerSuite(Test::newInstance<TestMacros>, "test-macros", "Tests all available test-macros");
    Test::registerSuite(Test::newInstance<TestFormat>, "test-format", "Tests the various output formats");
    Test::registerSuite(Test::newInstance<TestOutputs>, "test-outputs", "Tests the various output types");
    Test::registerSuite(Test::newInstance<TestParallelSuite>, "test-parallel", "Tests the parallel test suite");
    Test::registerSuite(Test::newInstance<TestAssertions>, "test-assertions", "Tests the available TEST_XXX assertions");
    Test::registerSuite(Test::newInstance<Story1>, "story1", "Runs the first BDD story", false);
    Test::registerSuite(Test::newInstance<Story2>, "story2", "Runs the second BDD story");
    Test::registerSuite(Test::newInstance<Story3>, "story3", "Runs the third BDD story");

    return Test::runSuites(argc, argv);
}

