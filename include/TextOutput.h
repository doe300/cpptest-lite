/* 
 * File:   TextOutput.h
 * Author: daniel
 *
 * Created on September 11, 2015, 7:07 PM
 */

#ifndef TEXTOUTPUT_H
#define	TEXTOUTPUT_H

#include "Output.h"

namespace Test
{

    class TextOutput : public Output
    {
    public:

        static const unsigned int Debug = 1;
        static const unsigned int Verbose = 2;
        static const unsigned int Terse = 3;

        TextOutput(const unsigned int mode);
        TextOutput(const unsigned int mode, std::ostream& stream);
        virtual ~TextOutput();

        virtual void initializeSuite(const std::string& suiteName, const unsigned int numTests) override;
        virtual void finishSuite(const std::string& suiteName, const unsigned int numTests, const unsigned int numPositiveTests, const std::chrono::microseconds totalDuration) override;

        virtual void initializeTestMethod(const std::string& suiteName, const std::string& methodName, const std::string& argString) override;
        virtual void finishTestMethod(const std::string& suiteName, const std::string& methodName, const bool withSuccess) override;


        virtual void printSuccess(const Assertion& assertion) override;
        virtual void printFailure(const Assertion& assertion) override;
        virtual void printException(const std::string& suiteName, const std::string& methodName, const std::exception& ex) override;

    protected:
        const unsigned int mode;
        std::ostream& stream;
    };

}

#endif	/* TEXTOUTPUT_H */

