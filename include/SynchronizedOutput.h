/* 
 * File:   SynchronizedOutput.h
 * Author: daniel
 *
 * Created on October 16, 2015, 11:44 AM
 */

#ifndef SYNCHRONIZEDOUTPUT_H
#define	SYNCHRONIZEDOUTPUT_H

#include <mutex>    //std::mutex

#include "Output.h"

namespace Test
{

    /*!
     * An Output which synchronizes all access to the underlying output making it safe for usage in a multi-threaded environment
     */
    class SynchronizedOutput : public Output
    {
    public:
        SynchronizedOutput(Output& realOutput);
        SynchronizedOutput(const SynchronizedOutput& orig) = delete;
        virtual ~SynchronizedOutput();
        
        void initializeSuite(const std::string& suiteName, const unsigned int numTests) override;
        void finishSuite(const std::string& suiteName, const unsigned int numTests, const unsigned int numPositiveTests, const std::chrono::microseconds totalDuration) override;
        void initializeTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString) override;
        void finishTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString, const bool withSuccess) override;
        
        void printException(const std::string& suiteName, const std::string& methodName, const std::string& argString, const std::exception& ex) override;
        void printSuccess(const Assertion& assertion) override;
        void printFailure(const Assertion& assertion) override;
    private:
        Output& realOutput;
        std::mutex outputMutex;
    };

}

#endif	/* SYNCHRONIZEDOUTPUT_H */

