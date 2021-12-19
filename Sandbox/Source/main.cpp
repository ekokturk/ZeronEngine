// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/Window.h"
#include "Graphics/API/D3D/11/GraphicsD3D11.h"

#include "Core/Types/Color.h"
#include "Core/Math/Vec3.h"

int main(int argc, char** argv) {

	using namespace Zeron;
	std::vector<std::unique_ptr<Window>> windows;
	windows.push_back(Window::CreatePlatformWindow(WindowAPI::SDL, WindowConfig("SDL",800,600,0)));
	windows.push_back(Window::CreatePlatformWindow(WindowAPI::GLFW, WindowConfig("GLFW",800,600,0)));
	windows.push_back(Window::CreatePlatformWindow(WindowAPI::Win32, WindowConfig("Win32",800,600,0)));

	auto gfx = std::make_unique<GraphicsD3D11>();
	gfx->Init();
	for (auto& window : windows) {
		window->Init();
		gfx->CreateGraphicsContext(window.get());
	}

	const std::vector v1 = {
	Vertex{{-.5f, -.5f, 1.f},{1.f,0,0}},
	Vertex{{0, .5f, 0.f},{1.f,0,0}},
	Vertex{{.5f, -.5f, 1.f},{1.f,0,0}},
	};

	const std::vector v2 = {
		Vertex{{-.3f, -.3f, 0.f},{1.f,1.f,0}},
		Vertex{{0, .3f, 0.f},{1.f,1.f,0}},
		Vertex{{.3f, -.3f, 1.f},{1.f,1.f,0}},
	};

	gfx->CreateVertexBuffer(v1);
	gfx->CreateVertexBuffer(v2);

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

		gfx->RenderFrame();
	}

	return 0;
}