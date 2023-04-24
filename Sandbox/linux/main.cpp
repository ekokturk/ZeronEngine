// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <SampleVulkan.h>

#include <Platform/API/Linux/PlatformLinux.h>

#include <Graphics/Graphics.h>
#include <Platform/EntryPoint.h>
#include <Platform/Platform.h>
#include <Platform/Window.h>

bool SandboxMain(Zeron::Platform& platform, const Zeron::CommandLineArgs& args)
{
	auto graphicsVulkan = Zeron::Gfx::Graphics::CreateGraphics(Zeron::Gfx::GraphicsType::Vulkan);

	graphicsVulkan->Init();

	Sandbox::SampleRunner runner;
	runner.AddSample<SampleVulkan::SampleInstance>(graphicsVulkan.get(), platform.CreatePlatformWindow({ "Vulkan - GLFW", 800, 600, 0, false, Zeron::WindowAPI::GLFW }));
	runner.AddSample<SampleVulkan::SampleInstance>(graphicsVulkan.get(), platform.CreatePlatformWindow({ "Vulkan - GLFW", 800, 600, 0, false, Zeron::WindowAPI::GLFW }));

	bool isRunning = true;
	while (isRunning) {
		platform.Update();
		isRunning &= runner.RunAll(Sandbox::SampleRunner::RunCondition::SingleSuccess);
	}

	return true;
}

ZERON_DECLARE_ENTRY_POINT(SandboxMain)