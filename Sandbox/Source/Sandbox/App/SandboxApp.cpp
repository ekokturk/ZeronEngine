// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Sandbox/App/SandboxApp.h>

#include <Graphics/Graphics.h>
#include <Graphics/GraphicsTypes.h>
#include <Platform/Platform.h>
#include <Platform/WindowConfig.h>
#include <Sandbox/App/ISandboxInstance.h>
#include <Sandbox/Samples/WindowInstance.h>

#include <Script/ScriptEngine.h>

Sandbox::Application::Application(Zeron::Platform& platform)
	: mPlatform(platform)
{
	mGraphics.emplace(Zeron::Gfx::GraphicsType::Vulkan, Zeron::Gfx::Graphics::CreateGraphics(Zeron::Gfx::GraphicsType::Vulkan));
#if ZE_PLATFORM_WIN32
	mGraphics.emplace(Zeron::Gfx::GraphicsType::Direct3D11, Zeron::Gfx::Graphics::CreateGraphics(Zeron::Gfx::GraphicsType::Direct3D11));
#endif
}

Sandbox::Application::~Application() = default;

void Sandbox::Application::Run()
{
	_init();

	int frameCount = 0;
	Zeron::Time::Timer<float, Zeron::Time::Seconds> timer;
	Zeron::Time::TickTimer ticker(Zeron::Time::Seconds(1.L / 60));

#if ZE_SCRIPT
	Zeron::Script::ScriptEngine scriptEngine;
#endif

	bool isRunning = true;
	while (isRunning) {
		const uint64_t tickCount = ticker.Tick();

		_update();

		for (uint64_t i = 0; i < tickCount; ++i) {
			_tick();
		}

		++frameCount;
		if (timer.HasTimeElapsed(1.f)) {
			ZE_LOG("{} FPS", frameCount);
			frameCount = 0;
			timer.Reset();
		}

		bool hasRunningSamples = false;
		mSampleRunner.ForEach([&](ISandboxInstance& sample) {
			hasRunningSamples |= sample.IsRunning();
		});
		isRunning = hasRunningSamples;
	}
	mSampleRunner.Destroy();
}

void Sandbox::Application::_init()
{
	for (auto& [type, graphics] : mGraphics) {
		graphics->Init();
	}

#if ZE_PLATFORM_ANDROID
	mSampleRunner.AddSample<WindowInstance>(mGraphics[Zeron::Gfx::GraphicsType::Vulkan].get(), mPlatform.GetMainWindow());
#else
#	if ZE_PLATFORM_WIN32
	// mSampleRunner.AddSample<WindowInstance>(
	//	mGraphics[Zeron::Gfx::GraphicsType::Direct3D11].get(), mPlatform.CreatePlatformWindow({ "D3D11 - Win32", 800, 600, 0, false, Zeron::WindowAPI::Win32 })
	//);
	// mSampleRunner.AddSample<WindowInstance>(
	//	mGraphics[Zeron::Gfx::GraphicsType::Vulkan].get(), mPlatform.CreatePlatformWindow({ "Vulkan - Win32", 800, 600, 0, false, Zeron::WindowAPI::Win32 })
	//);
	mSampleRunner.AddSample<WindowInstance>(
		mGraphics[Zeron::Gfx::GraphicsType::Direct3D11].get(), mPlatform.CreatePlatformWindow({ "D3D11 - GLFW", 1200, 800, 0, false, Zeron::WindowAPI::GLFW })
	);
#	endif
	mSampleRunner.AddSample<WindowInstance>(
		mGraphics[Zeron::Gfx::GraphicsType::Vulkan].get(), mPlatform.CreatePlatformWindow({ "Vulkan - GLFW", 1200, 800, 0, false, Zeron::WindowAPI::GLFW })
	);
#endif
}

void Sandbox::Application::_update()
{
	mPlatform.Update();

	mSampleRunner.ForEach([](ISandboxInstance& sample) {
		if (!sample.IsInit()) {
			sample.Init();
		}
		sample.Update();
	});
}

void Sandbox::Application::_tick()
{
	mSampleRunner.ForEach([](ISandboxInstance& sample) {
		sample.Tick();
	});
}