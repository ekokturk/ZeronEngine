// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/EntryPoint.h>
#include <Platform/Platform.h>
#include <Sandbox/App/SandboxApp.h>

bool SandboxMain(Zeron::Platform& platform, const Zeron::CommandLineArgs& args)
{
	ZE_LOG_FILE("debug.txt");
	{
		Sandbox::Application app(platform);
		app.Run();
	}
	return true;
}

ZERON_DECLARE_ENTRY_POINT(SandboxMain)