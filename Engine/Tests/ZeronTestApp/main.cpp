// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/EntryPoint.h>
#include <Platform/Platform.h>
#include <ZeronTestFramework.h>

class TestListener : public ::testing::TestEventListener {
  public:
	TestListener() = default;

	void OnTestProgramStart(const ::testing::UnitTest&) override
	{
		mFailedTests.clear();
		mTotalCount = 0;
		mFailedCount = 0;
	}

	void OnTestIterationStart(const ::testing::UnitTest&, int) override {}

	void OnEnvironmentsSetUpStart(const ::testing::UnitTest&) override {}

	void OnEnvironmentsSetUpEnd(const ::testing::UnitTest&) override {}

	void OnTestStart(const ::testing::TestInfo& testInfo) override
	{
		ZE_LOGI("[ RUN      ] {}.{}", testInfo.test_suite_name(), testInfo.name());
		++mTotalCount;
	}

	void OnTestPartResult(const ::testing::TestPartResult& result) override
	{
		if (result.failed()) {
			ZE_LOGE("[  FAILED  ] {}:{}\n{}", result.file_name(), result.line_number(), _sanitizeLogString(result.summary()));
		}
		else {
			ZE_LOGI("[       OK ] {}:{}\n{}", result.file_name(), result.line_number(), result.summary());
		}
	}

	void OnTestEnd(const ::testing::TestInfo& testInfo) override
	{
		if (testInfo.result()->Failed()) {
			++mFailedCount;
			mFailedTests.emplace_back(std::string(testInfo.test_suite_name()) + "." + testInfo.name());
			ZE_LOGE("[  FAILED  ] {}:{}", testInfo.test_suite_name(), testInfo.name());
		}
		else {
			ZE_LOGI("[       OK ] {}:{}", testInfo.test_suite_name(), testInfo.name());
		}
	}

	void OnEnvironmentsTearDownStart(const ::testing::UnitTest&) override {}

	void OnEnvironmentsTearDownEnd(const ::testing::UnitTest&) override {}

	void OnTestIterationEnd(const ::testing::UnitTest& unitTest, int) override
	{
		const int passedCount = unitTest.total_test_count() - mFailedCount;
		const float successRate = 100.0f * passedCount / unitTest.total_test_count();

		std::string out = "\n";
		out += "==============================\n";
		out += Zeron::Util::Format("TESTS RUN:    {}\n", unitTest.total_test_count());
		out += Zeron::Util::Format("PASSED:       {}\n", passedCount);
		out += Zeron::Util::Format("FAILED:       {}\n", mFailedCount);
		out += Zeron::Util::Format("SUCCESS RATE: {}%\n", successRate);

		if (!mFailedTests.empty()) {
			out += "FAILED TESTS:\n";
			for (const auto& name : mFailedTests) {
				out += Zeron::Util::Format(" - {}\n", name);
			}
		}
		out += "==============================\n";

		ZE_LOGI("{}", out);
	}

	void OnTestProgramEnd(const ::testing::UnitTest&) override {}

  private:
	std::string _sanitizeLogString(std::string_view input)
	{
		std::string result;
		result.reserve(input.size());

		for (char c : input) {
			if (c == '{' || c == '}') {
				result += c;
			}
			result += c;
		}
		return result;
	}

	int mTotalCount = 0;
	int mFailedCount = 0;
	std::vector<std::string> mFailedTests;
};


bool TestAppMain(Zeron::Platform& platform, const Zeron::CommandLineArgs& args)
{
	ZE_LOG_FILE("output.txt");

	Zeron::Locator::Register<Zeron::Platform>(&platform);

	::ZeronTestFramework::TestRunner runner;
	const auto& argv = args.GetRawArgs();

	runner.Init(argv.first, argv.second);

	::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
	delete listeners.Release(listeners.default_result_printer());
	listeners.Append(new TestListener());

	runner.Run();
	runner.Destroy();
	return runner.GetErrorCode() == 0;
}

ZERON_DECLARE_ENTRY_POINT(TestAppMain)