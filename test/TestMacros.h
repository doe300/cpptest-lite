#pragma once

#include <cmath>
#include <exception>

#include "cpptest.h"

class TestMacros : public Test::Suite {
public:
  TestMacros();

  void testMethodNoArgs();
  void testMethodWithCStringArg(char *arg);
  void testMethodWithIntArg(int arg);
  void testMethodWithPointerArg(void *arg);
  void testMethodWithStringArg(std::string string);
  void testMethodWithVarargs1(double d, int i);
  void testMethodWithVarargs2(std::string s, int i, double d);

  void testFailureMessages();

private:
  void throwsException();
  void throwsInt();
};
