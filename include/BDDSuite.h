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
        
        BDDSuite();
        BDDSuite(const std::string& suiteName);
        virtual ~BDDSuite();
        
        void runScenario(std::string scenarioName);
        
    protected:
        std::vector<Scenario> scenarios;
    };
}
#endif /* BDDSUITE_H */

