// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/Window.h"

#include "Core/Types/Color.h"

int main(int argc, char** argv) {

	using namespace Zeron;
	std::vector<std::unique_ptr<Window>> windows;
	//windows.push_back(Window::CreatePlatformWindow(WindowAPI::SDL, WindowConfig("SDL",800,600,0)));
	//windows.push_back(Window::CreatePlatformWindow(WindowAPI::GLFW, WindowConfig("GLFW",800,600,0)));
	//windows.push_back(Window::CreatePlatformWindow(WindowAPI::Win32, WindowConfig("Win32",800,600,0)));

	for (auto& window : windows) {
		window->Init();
	}

	Color color = Color::Red;
	ZE_LOG("{:x}", color.HexRGB());

	bool isRunning = true;
	while (isRunning) {
		for(auto& window : windows) { 
			window->BeginFrame();
			WindowEvent e;
			while (auto e = window->GetNextEvent()) {
				if (e->GetID() == WindowEventID::WindowCreated) {
					bool test = false;
				}
				if (e->GetID() == WindowEventID::WindowClosed) {
					isRunning = false;
				}
				if (e->GetID() == WindowEventID::KeyDown) {
					WindowEvent_KeyDown& procEvnt = static_cast<WindowEvent_KeyDown&>(*e);
					std::cout << window->GetName() << " " << procEvnt.GetEventName() << " " << procEvnt.mCode.ToString() << std::endl;
					if (procEvnt.mCode == KeyCode::F) {
						window->SetFullScreen(!window->IsFullScreen());
					}
					if (procEvnt.mCode == KeyCode::H) {
						window->SetHidden();
					}
					if (procEvnt.mCode == KeyCode::G) {
						window->SetAspectRatio(16,9);
					}

					if (procEvnt.mCode == KeyCode::N1) {
						window->SetFullScreenType(FullScreenType::Borderless);
					}

					if (procEvnt.mCode == KeyCode::N2) {
						window->SetFullScreenType(FullScreenType::Monitor);
					}
				}
				if (e->GetID() == WindowEventID::KeyUp) {
					WindowEvent_KeyUp& procEvnt = static_cast<WindowEvent_KeyUp&>(*e);
					std::cout << window->GetName() << " " << procEvnt.GetEventName() << " " << procEvnt.mCode.ToString() << std::endl;
				}
				if (e->GetID() == WindowEventID::MouseButtonDown) {
					WindowEvent_MouseDown& procEvnt = static_cast<WindowEvent_MouseDown&>(*e);
					std::cout << window->GetName() << procEvnt.GetEventName() << " " << procEvnt.mCode.ToString() << std::endl;
				}
				else if (e->GetID() == WindowEventID::TextChar) {
					WindowEvent_TextChar& procEvnt = static_cast<WindowEvent_TextChar&>(*e);
					ZE_LOG("{}", procEvnt.mCharacter);
					//std::cout << procEvnt.GetEventName() << " " << procEvnt.mCharacter << std::endl;
				}
				else if (e->GetID() == WindowEventID::MouseEnter) {
					WindowEvent_MouseEnter& procEvnt = static_cast<WindowEvent_MouseEnter&>(*e);
					std::cout << procEvnt.GetEventName()  << std::endl;
				}
				else if (e->GetID() == WindowEventID::MouseExit) {
					WindowEvent_MouseExit& procEvnt = static_cast<WindowEvent_MouseExit&>(*e);
					std::cout << procEvnt.GetEventName()  << std::endl;
				}
			}
			window->EndFrame();
		}


	}

	return 0;
}