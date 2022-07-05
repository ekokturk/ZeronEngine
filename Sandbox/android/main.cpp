// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Window/Window.h>

using namespace Zeron;

__attribute__ ((visibility("default"))) 
int main(int argc, char* argv[]) {
	auto window = Window::CreatePlatformWindow(WindowAPI::SDL, WindowConfig("Linux - SDL", 800, 600, 0));
	window->Init();

	ZE_LOGI("Launched Zeron Android App...");
	bool isRunning = true;
	while (isRunning) {
		std::cout << "Test";
		window->BeginFrame();
		while (auto e = window->GetNextEvent()) {
			if (e->GetID() == WindowEventID::WindowClosed) {
				isRunning = false;
			}
		}
		window->EndFrame();
	}
	return 0;
}