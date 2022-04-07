#pragma once

#include "BDDSuite.h"

//Defines a new story containing up to several scenarios
#define STORY(name) class name : public Test::BDDSuite { \
public: \
    name() : Test::BDDSuite(#name) {

//Defines a new story containing scenarios and allowing for local variables
#define STORY_WITH_MEMBERS(name, locals) class name : public Test::BDDSuite { \
private: \
    locals ; \
public: \
    name() : Test::BDDSuite(#name) {

//Required to mark the end of a story
#define END_STORY \
    } \
};

//Defines an new scenario
#define SCENARIO(name) \
    TEST_ADD_WITH_STRING(Test::BDDSuite::runScenario,#name); \
    scenarios.push_back({ #name,

//Required to mark the end of a scenario
#define END_SCENARIO  });

//Defines a precondition for a scenario to run
#define GIVEN(condition) [this]() -> bool{return condition;},

//Defines the actions to run
#define WHEN(actions) [this]() -> void{actions;},

//Defines a postcondition which should be met
#define THEN(condition) [this]() -> bool{return condition;}
