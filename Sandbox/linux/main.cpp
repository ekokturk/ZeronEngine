// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <SampleVulkan.h>

#include <Platform/Window.h>
#include <Platform/Platform.h>
#include <Graphics/Graphics.h>

int main(int argc, char** argv) {

	auto platform = Zeron::Platform::CreatePlatformInstance();
	auto graphicsVulkan = Zeron::Graphics::CreateGraphics(Zeron::GraphicsType::Vulkan);

	graphicsVulkan->Init();

	Sandbox::SampleRunner runner;
	runner.AddSample<SampleVulkan::SampleInstance>(graphicsVulkan.get(), platform->CreatePlatformWindow({ "Vulkan - GLFW", 800, 600, 0, false, Zeron::WindowAPI::GLFW }));
	runner.AddSample<SampleVulkan::SampleInstance>(graphicsVulkan.get(), platform->CreatePlatformWindow({ "Vulkan - SDL", 800, 600, 0, false, Zeron::WindowAPI::SDL }));

	bool isRunning = true;
	while(isRunning)
	{
		platform->Update();
		isRunning &= runner.RunAll(Sandbox::SampleRunner::RunCondition::SingleSuccess);
	}

	return 0;
}