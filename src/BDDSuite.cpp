/* 
 * File:   BDDSuite.cpp
 * Author: doe300
 * 
 * Created on June 29, 2016, 2:47 PM
 */

#include "BDDSuite.h"

using namespace Test;

BDDSuite::BDDSuite() : Suite(), scenarios()
{

}

BDDSuite::BDDSuite(const std::string& suiteName) : Suite(suiteName), scenarios()
{

}

BDDSuite::~BDDSuite()
{
}

void BDDSuite::runScenario(std::string scenarioName)
{
    const Scenario* scenario = nullptr;
    for(const Scenario& scen : scenarios)
    {
        if(scen.name.compare(scenarioName) == 0)
        {
            scenario = &scen;
            break;
        }
    }
    if(scenario == nullptr)
    {
        TEST_FAIL("Internal error, no such scenario found!");
        return;
    }
    TEST_ASSERT_MSG(scenario->precondition(), "Precondition failed!");
    TEST_THROWS_NOTHING_MSG(scenario->action(), "Scenario threw an exception!");
    TEST_ASSERT_MSG(scenario->postcondition(), "Postcondition failed!");
}
