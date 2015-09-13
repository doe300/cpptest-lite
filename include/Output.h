/* 
 * File:   Output.h
 * Author: daniel
 *
 * Created on September 10, 2015, 4:59 PM
 */

#ifndef OUTPUT_H
#define	OUTPUT_H

#include <chrono>
#include <string>
#include <exception>


namespace Test
{
    namespace Private
    {
        inline std::string getFileName(const std::string& file)
        {
            std::string fileName = file;
            unsigned long pos = fileName.find_last_of("\\");
            if(pos != std::string::npos)
                fileName = fileName.substr(pos + 1);
            pos = fileName.find_last_of("/");
            if(pos != std::string::npos)
                fileName = fileName.substr(pos + 1);
            return fileName;
        }
    };
    
    struct Assertion
    {
        std::string suite;
        const std::string file;
        std::string method;
        const int lineNumber;
        std::string errorMessage;
        const std::string userMessage;

        Assertion(const char* fileName, const int lineNumber, std::string errorMessage, const char* userMessage) :
            suite(""), file(fileName), method(""), lineNumber(lineNumber), errorMessage(errorMessage), userMessage(userMessage)
        { }

        Assertion(const char* fileName, const int lineNumber, const char* userMessage) :
            suite(""), file(fileName), method(""), lineNumber(lineNumber), errorMessage(""), userMessage(userMessage)
        { }

        Assertion(const char* fileName, const int lineNumber) :
            suite(""), file(fileName), method(""), lineNumber(lineNumber), errorMessage(""), userMessage("")
        { }
    };

class Output
{
public:
    Output(const Output& orig) = delete;
    virtual ~Output() {}
    
    virtual void initializeSuite(const std::string& suiteName, const unsigned int numTests) {};
    virtual void finishSuite(const std::string& suiteName, const unsigned int numTests, const unsigned int numPositiveTests, const std::chrono::microseconds totalDuration) {};
    
    virtual void initializeTestMethod(const std::string& suiteName, const std::string& methodName) {};
    virtual void finishTestMethod(const std::string& suiteName, const std::string& methodName, const bool withSuccess) {};
    
    virtual void printSuccess(const Assertion assertion) {};
    virtual void printFailure(const Assertion assertion) {};
    
    virtual void printException(const std::string& suiteName, const std::string& methodName, const std::exception& ex) {};
    
protected:
    Output() {}

    inline double prettifyPercentage(const double part, const double whole) const 
    {
        int tmp = (part/whole) * 10000;
        return tmp / 100.0;
    }
};

};

#endif	/* OUTPUT_H */

