// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <SampleVulkan.h>
#include "Platform/CommandLineArgs.h"

#include <Graphics/Graphics.h>
#include <Platform/EntryPoint.h>
#include <Platform/Platform.h>

using namespace Zeron;

bool ZeronMain(Platform& platform, const CommandLineArgs& args)
{
	auto graphicsVulkan = Zeron::Graphics::CreateGraphics(Zeron::GraphicsType::Vulkan);
	graphicsVulkan->Init();

	Sandbox::SampleRunner runner;
	runner.AddSample<SampleVulkan::SampleInstance>(graphicsVulkan.get(), platform.GetMainWindow());

	bool isRunning = true;
	while (isRunning) {
		platform.Update();
		runner.RunAll(Sandbox::SampleRunner::RunCondition::SingleSuccess);
	}
	return true;
}

ZERON_DECLARE_ENTRY_POINT(ZeronMain)