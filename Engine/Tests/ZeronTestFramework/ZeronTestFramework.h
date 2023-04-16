// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <gtest/gtest.h>

namespace ZeronTestFramework
{

	class TestRunner {
	  public:
		TestRunner();
		virtual ~TestRunner() = default;

		void Init(int argc, char** argv);
		void Run();
		void Destroy();

		int GetErrorCode() const { return mErrorCode; }

	  protected:
		virtual void OnInit(int argc, char** argv) {}
		virtual void OnRun() {}
		virtual void OnDestroy() {}

		int mErrorCode;
	};
}

#define ZE_GENERATE_DEFAULT_TEST_RUNNER()        \
	int main(int argc, char** argv)              \
	{                                            \
		::ZeronTestFramework::TestRunner runner; \
		runner.Init(argc, argv);                 \
		runner.Run();                            \
		runner.Destroy();                        \
		return runner.GetErrorCode();            \
	}