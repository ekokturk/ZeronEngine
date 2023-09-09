// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Samples/SampleGraphics.h>

#include <Platform/EntryPoint.h>
#include <Platform/Platform.h>
#include <Platform/Window.h>

#include <Graphics/Graphics.h>

bool SandboxMain(Zeron::Platform& platform, const Zeron::CommandLineArgs& args)
{
	ZE_LOG_FILE("debug.txt");

	Sandbox::SampleRunner runner;
	auto graphicsVulkan = Zeron::Gfx::Graphics::CreateGraphics(Zeron::Gfx::GraphicsType::Vulkan);
	graphicsVulkan->Init();
#if ZE_PLATFORM_ANDROID
	runner.AddSample<SampleGraphics::SampleInstance>(graphicsVulkan.get(), platform.GetMainWindow());
#else
#	if ZE_PLATFORM_WIN32
	auto graphicsD3D11 = Zeron::Gfx::Graphics::CreateGraphics(Zeron::Gfx::GraphicsType::Direct3D11);
	graphicsD3D11->Init();
	runner.AddSample<SampleGraphics::SampleInstance>(graphicsD3D11.get(), platform.CreatePlatformWindow({ "D3D11 - Win32", 800, 600, 0, false, Zeron::WindowAPI::Win32 }));
	runner.AddSample<SampleGraphics::SampleInstance>(graphicsVulkan.get(), platform.CreatePlatformWindow({ "Vulkan - Win32", 800, 600, 0, false, Zeron::WindowAPI::Win32 }));
	runner.AddSample<SampleGraphics::SampleInstance>(graphicsD3D11.get(), platform.CreatePlatformWindow({ "D3D11 - GLFW", 800, 600, 0, false, Zeron::WindowAPI::GLFW }));
#	endif
	runner.AddSample<SampleGraphics::SampleInstance>(graphicsVulkan.get(), platform.CreatePlatformWindow({ "Vulkan - GLFW", 800, 600, 0, false, Zeron::WindowAPI::GLFW }));
#endif

	int frameCount = 0;
	Zeron::Time::Timer<float, Zeron::Time::Seconds> timer;
	Zeron::Time::TickTimer ticker(Zeron::Time::Seconds(1.L / 60));

	bool isRunning = true;
	while (isRunning) {
		const uint64_t tickCount = ticker.Tick();

		runner.ForEach([](Sandbox::ISampleInstance& sample) {
			if (!sample.IsInit()) {
				sample.Init();
			}
		});

		platform.Update();

		runner.ForEach([](Sandbox::ISampleInstance& sample) {
			sample.Update();
		});

		// Fixed Update
		for (uint64_t i = 0; i < tickCount; ++i) {
			runner.ForEach([](Sandbox::ISampleInstance& sample) {
				sample.FixedUpdate();
			});
		}

		++frameCount;
		if (timer.hasTimeElapsed(1.f)) {
			ZE_LOG("{} FPS", frameCount);
			frameCount = 0;
			timer.Reset();
		}

		bool hasRunningSamples = false;
		runner.ForEach([&](Sandbox::ISampleInstance& sample) {
			hasRunningSamples |= sample.IsRunning();
		});
		isRunning = hasRunningSamples;
	}

	runner.Destroy();

	return true;
}

ZERON_DECLARE_ENTRY_POINT(SandboxMain)