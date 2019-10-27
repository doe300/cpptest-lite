/* 
 * File:   BDDSuite.h
 * Author: doe300
 *
 * Created on June 29, 2016, 2:47 PM
 */

#ifndef BDDSUITE_H
#define BDDSUITE_H

#include "cpptest.h"

namespace Test
{

	/*!
	 * Test Suite for behavior driven development
	 *
	 * \since 0.6
	 */
	class BDDSuite : public Suite
	{
	public:

		using Condition = std::function<bool()>;
		using Action = std::function<void()>;

		struct Scenario
		{
			std::string name;
			Condition precondition;
			Action action;
			Condition postcondition;
		};

		BDDSuite() = default;
		explicit BDDSuite(const std::string& suiteName);
		BDDSuite(const BDDSuite&) = delete;
		BDDSuite(BDDSuite&&) noexcept = default;
		~BDDSuite() noexcept override = default;

		BDDSuite& operator=(const BDDSuite&) = delete;
		BDDSuite& operator=(BDDSuite&&) = default;

		void runScenario(std::string scenarioName);

	protected:
		std::vector<Scenario> scenarios;
	};
} // namespace Test
#endif /* BDDSUITE_H */

