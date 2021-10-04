// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/Window.h"

int main(int argc, char** argv) {

	using namespace Zeron;
	auto window = Window::CreateWindow(WindowType::GLFW, WindowConfig());
	while (true) {
		window->BeginFrame();
		window->EndFrame();
	}

	return 0;
}