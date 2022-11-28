// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <SampleD3D11.h>
#include <SampleVulkan.h>

#include <Window/Window.h>

int main(int argc, char** argv) {

	auto windowDx = Zeron::Window::CreatePlatformWindow(Zeron::WindowAPI::Win32, Zeron::WindowConfig("D3D11", 800, 600, 0));
	auto windowVk = Zeron::Window::CreatePlatformWindow(Zeron::WindowAPI::Win32, Zeron::WindowConfig("Vulkan", 800, 600, 0));

	while(
		SampleD3D11::Run(windowDx.get()) &&	
		SampleVulkan::Run(windowVk.get()) 
		) 
	{
	}

	return 0;
}