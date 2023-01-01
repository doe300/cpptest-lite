#pragma once

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
		ParallelSuite& operator=(ParallelSuite&&) = default;

		bool run(Output& output, const std::vector<TestMethodInfo>& selectedMethods, bool continueAfterFail) override;

	private:
		bool runSuite(unsigned int suiteIndex, const std::vector<TestMethodInfo>& selectedMethods);
	};
} // namespace Test
