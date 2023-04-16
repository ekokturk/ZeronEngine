// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <SampleD3D11.h>
#include <SampleVulkan.h>

#include <Platform/EntryPoint.h>
#include <Platform/Platform.h>
#include <Platform/Window.h>

#include <Graphics/Graphics.h>

bool SandboxMain(Zeron::Platform& platform, const Zeron::CommandLineArgs& args)
{
	ZE_LOG_FILE("debug.txt");

	auto graphicsD3D11 = Zeron::Graphics::CreateGraphics(Zeron::GraphicsType::Direct3D11);
	auto graphicsVulkan = Zeron::Graphics::CreateGraphics(Zeron::GraphicsType::Vulkan);

	graphicsD3D11->Init();
	graphicsVulkan->Init();

	Sandbox::SampleRunner runner;
	runner.AddSample<SampleD3D11::SampleInstance>(graphicsD3D11.get(), platform.CreatePlatformWindow({ "D3D11 - Win32", 800, 600, 0, false, Zeron::WindowAPI::Win32 }));
	runner.AddSample<SampleD3D11::SampleInstance>(graphicsD3D11.get(), platform.CreatePlatformWindow({ "D3D11 - GLFW", 800, 600, 0, false, Zeron::WindowAPI::GLFW }));
	runner.AddSample<SampleVulkan::SampleInstance>(graphicsVulkan.get(), platform.CreatePlatformWindow({ "Vulkan - GLFW", 800, 600, 0, false, Zeron::WindowAPI::GLFW }));
	runner.AddSample<SampleVulkan::SampleInstance>(graphicsVulkan.get(), platform.CreatePlatformWindow({ "Vulkan - Win32", 800, 600, 0, false, Zeron::WindowAPI::Win32 }));

	int frameCount = 0;
	Zeron::Time::Timer<float, Zeron::Time::Seconds> timer;
	Zeron::Time::TickTimer ticker(Zeron::Time::Seconds(1.L / 60));

	bool isRunning = true;
	while (isRunning) {
		const uint64_t tickCount = ticker.Tick();

		for (uint64_t i = 0; i < tickCount; ++i) {
			// Fixed Update
		}

		platform.Update();
		isRunning &= runner.RunAll(Sandbox::SampleRunner::RunCondition::AllSuccess);

		++frameCount;
		if (timer.hasTimeElapsed(1.f)) {
			ZE_LOG("{} FPS", frameCount);
			frameCount = 0;
			timer.Reset();
		}
	}

	return true;
}

ZERON_DECLARE_ENTRY_POINT(SandboxMain)