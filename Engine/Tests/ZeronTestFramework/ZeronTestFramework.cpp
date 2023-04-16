// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <ZeronTestFramework.h>

namespace ZeronTestFramework
{
	TestRunner::TestRunner()
		: mErrorCode(0)
	{}

	void TestRunner::Init(int argc, char** argv)
	{
		::testing::InitGoogleTest(&argc, argv);
		OnInit(argc, argv);
	}

	void TestRunner::Run()
	{
		mErrorCode = RUN_ALL_TESTS();
		OnRun();
	}

	void TestRunner::Destroy()
	{
		OnDestroy();
	}
}