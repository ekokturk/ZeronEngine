// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <SampleVulkan.h>

#include <Window/Window.h>

using namespace Zeron;

int main(int argc, char** argv) {

	auto windowVk = Zeron::Window::CreatePlatformWindow(Zeron::WindowAPI::GLFW, Zeron::WindowConfig("Vulkan", 800, 600, 0));

	while(
		SampleVulkan::Run(windowVk.get())
		) 
	{
	}
	return 0;
}