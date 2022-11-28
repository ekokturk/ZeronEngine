// Copyright (C) Eser Kokturk. All Rights Reserved.

#include "Core/Logger.h"
#include <Window/Window.h>

#include <SampleVulkan.h>

using namespace Zeron;

__attribute__ ((visibility("default"))) 
int main(int argc, char** argv) {

	auto windowVk = Zeron::Window::CreatePlatformWindow(Zeron::WindowAPI::SDL, Zeron::WindowConfig("Vulkan", 800, 600, 0));

	while(
		SampleVulkan::Run(windowVk.get())
		) 
	{
	}
	return 0;
}