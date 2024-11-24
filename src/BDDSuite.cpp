#include "BDDSuite.h"

using namespace Test;

BDDSuite::BDDSuite(const std::string& suiteName) : Suite(suiteName)
{

}

void BDDSuite::runScenario(const std::string scenarioName)
{
	const Scenario* scenario = nullptr;
	for(const Scenario& scen : scenarios)
	{
		if(scen.name == scenarioName)
		{
			scenario = &scen;
			break;
		}
	}
	if(scenario == nullptr)
	{
		TEST_ABORT("Internal error, no such scenario found!");
	}
	TEST_ASSERT_MSG(scenario->precondition(), "Precondition failed!");
	TEST_THROWS_NOTHING_MSG(scenario->action(), "Scenario threw an exception!");
	TEST_ASSERT_MSG(scenario->postcondition(), "Postcondition failed!");
}
