/* 
 * File:   ParallelSuite.h
 * Author: daniel
 *
 * Created on September 13, 2015, 5:14 PM
 */

#ifndef PARALLELSUITE_H
#define	PARALLELSUITE_H

#include "SynchronizedOutput.h"
#include "TestSuite.h"

#include <future>
#include <thread>

namespace Test
{
	/*!
	 * A suite which calls every sub-suite in an own thread and therefore in parallel.
	 * ParallelSuite does not support adding test-methods directly to the suite.
	 * All test-methods must be added via a sub-suite
	 */
	class ParallelSuite : public Suite
	{
	public:
		ParallelSuite() = default;
		explicit ParallelSuite(const std::string& suiteName);
		ParallelSuite(const ParallelSuite&) = delete;
		ParallelSuite(ParallelSuite&&) noexcept = default;
		~ParallelSuite() override;

		ParallelSuite& operator=(const ParallelSuite&) = delete;
		ParallelSuite& operator=(ParallelSuite&&) noexcept = default;

		bool run(Output& output, bool continueAfterFail = true) override;

	private:
		bool runSuite(unsigned int suiteIndex);
	};
} // namespace Test

#endif	/* PARALLELSUITE_H */

