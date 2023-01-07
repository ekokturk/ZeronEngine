// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <SampleD3D11.h>
#include <SampleVulkan.h>

#include <Platform/Window.h>
#include <Platform/Platform.h>
#include <Graphics/Graphics.h>

int main(int argc, char** argv) {

	auto platform = Zeron::Platform::CreatePlatformInstance();
	auto graphicsD3D11 = Zeron::Graphics::CreateGraphics(Zeron::GraphicsType::Direct3D11);
	auto graphicsVulkan = Zeron::Graphics::CreateGraphics(Zeron::GraphicsType::Vulkan);

	graphicsD3D11->Init();
	graphicsVulkan->Init();

	Sandbox::SampleRunner runner;
	runner.AddSample<SampleD3D11::SampleInstance>(graphicsD3D11.get(), platform->CreatePlatformWindow({ "Win32", 800, 600, 0, false, Zeron::WindowAPI::Win32 }));
	runner.AddSample<SampleD3D11::SampleInstance>(graphicsD3D11.get(), platform->CreatePlatformWindow({ "GLFW", 800, 600, 0, false, Zeron::WindowAPI::GLFW }));
	runner.AddSample<SampleD3D11::SampleInstance>(graphicsD3D11.get(), platform->CreatePlatformWindow({ "SDL", 800, 600, 0, false, Zeron::WindowAPI::SDL }));
	runner.AddSample<SampleVulkan::SampleInstance>(graphicsVulkan.get(), platform->CreatePlatformWindow({ "Vulkan", 800, 600, 0, false, Zeron::WindowAPI::Win32 }));
	runner.AddSample<SampleVulkan::SampleInstance>(graphicsVulkan.get(), platform->CreatePlatformWindow({ "Vulkan", 800, 600, 0, false, Zeron::WindowAPI::Win32 }));

	bool isRunning = true;
	while(isRunning)
	{
		platform->Update();
		isRunning &= runner.RunAll(Sandbox::SampleRunner::RunCondition::AllSuccess);
	}

	return 0;
}