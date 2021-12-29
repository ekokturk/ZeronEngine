// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/Window.h"
#include "Graphics/API/D3D/11/GraphicsD3D11.h"

#include "Core/Types/Color.h"
#include "Core/Math/Vec3.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/SwapChain.h"
#include "Graphics/Buffer.h"
#include "Core/Math/Mat4.h"
#include "glm/glm.hpp"
#include "Renderer/Camera.h"

using namespace Zeron;


struct CBData
{
	Mat4 mMatrix;
};

void TestWindow()
{

	std::array<std::shared_ptr<SwapChain>, 3> swapChainList;

	std::vector<std::unique_ptr<Window>> windows;
	windows.push_back(Window::CreatePlatformWindow(WindowAPI::SDL, WindowConfig("SDL", 800, 600, 0)));
	windows.push_back(Window::CreatePlatformWindow(WindowAPI::GLFW, WindowConfig("GLFW", 800, 600, 0)));
	windows.push_back(Window::CreatePlatformWindow(WindowAPI::Win32, WindowConfig("Win32", 800, 600, 0)));

	auto gfx = std::make_unique<GraphicsD3D11>();
	if (!gfx->Init()) {
		return;
	}
	auto ctx = gfx->GetImmediateContext();
	int i = 0;
	for (auto& window : windows) {
		window->Init();
		swapChainList[i] = gfx->CreateSwapChain(*window);
		i++;
	}

	const std::vector v1 = {
		Vertex{{-.5f, -.5f, 0.f},{0.f,1.f}},
		Vertex{{-.5f, .5f, 0.f},{0.f,0.f}},
		Vertex{{.5f, .5f, 0.f},{1.f,0}},
		Vertex{{.5f, -.5f, 0.f},{1.f,1.f}},
	};

	const std::vector<unsigned long> i1 = {
		0, 1, 2,
		0, 2, 3
	};

	CBData dataCB;
	Camera camera;
	camera.SetPosition({ 0.f, 0, -2.f });

	auto vertexBuffer = gfx->CreateVertexBuffer(v1);
	auto indexBuffer = gfx->CreateIndexBuffer(i1);
	auto constantBuffer = gfx->CreateConstantBuffer<CBData>(dataCB);
	auto shader = gfx->CreateShader("Default");
	auto texture = gfx->CreateTexture("test_texture.png");

	Mat4 worldMatrix;
	
	bool isRunning = true;
	while (isRunning) {


		for (int k = 0; k < windows.size(); k++) {
			Window* window = windows[k].get();

			window->BeginFrame();
			WindowEvent e;
			while (auto e = window->GetNextEvent()) {
				if (e->GetID() == WindowEventID::WindowClosed) {
					isRunning = false;
				}
				if (e->GetID() == WindowEventID::KeyDown) {
					auto& procEvnt = static_cast<WindowEvent_KeyDown&>(*e);
					std::cout << window->GetName() << " " << procEvnt.GetEventName() << " " << procEvnt.mCode.ToString() << std::endl;
					if (procEvnt.mCode == KeyCode::F) {
						window->SetFullScreen(!window->IsFullScreen());
					}
					if (procEvnt.mCode == KeyCode::H) {
						window->SetHidden();
					}
					if (procEvnt.mCode == KeyCode::G) {
						window->SetAspectRatio(16, 9);
					}
					if (procEvnt.mCode == KeyCode::C) {
						ctx->SetFillMode(true);
					}
					if (procEvnt.mCode == KeyCode::V) {
						ctx->SetFillMode(false);
					}
					if (procEvnt.mCode == KeyCode::N1) {
						window->SetFullScreenType(FullScreenType::Borderless);
					}
					if (procEvnt.mCode == KeyCode::N2) {
						window->SetFullScreenType(FullScreenType::Monitor);
					}

					if (procEvnt.mCode == KeyCode::W) {
						//camera.Move({ 0,0,-.3f });
						camera.Move(camera.GetForwardDir() * -.25f);
					}
					if (procEvnt.mCode == KeyCode::S) {
						//camera.Move({ 0,0,.3f });
						camera.Move(camera.GetForwardDir() * .25f);
					}
					if (procEvnt.mCode == KeyCode::A) {
						//camera.Move({ -1.f,0,0 });
						camera.Move(camera.GetRightDir() * -.25f);
					}
					if (procEvnt.mCode == KeyCode::D) {
						//camera.Move({ 1.f,0,0 });
						camera.Move(camera.GetRightDir() * .25f);
					}
					if (procEvnt.mCode == KeyCode::Q) {
						//camera.Move({ 0,-1.f,0 });
						camera.Move(camera.GetUpDir() * -.25f);
					}
					if (procEvnt.mCode == KeyCode::E) {
						//camera.Move({ 0,1.f,0 });
						camera.Move(camera.GetUpDir() * .25f);
					}
					
					if (procEvnt.mCode == KeyCode::Up) {
						camera.Rotate({ Math::ToRadians(15.f),0,0 });
					}
					if (procEvnt.mCode == KeyCode::Down) {
						camera.Rotate({ Math::ToRadians(-15.f),0,0 });
					}
					if (procEvnt.mCode == KeyCode::Left) {
						camera.Rotate({ 0,Math::ToRadians(-15.f),0 });
					}
					if (procEvnt.mCode == KeyCode::Right) {
						camera.Rotate({ 0,Math::ToRadians(15.f),0 });
					}
					if (procEvnt.mCode == KeyCode::RightShift) {
						camera.Rotate({ 0,0,Math::ToRadians(-15.f) });
					}
					if (procEvnt.mCode == KeyCode::RightControl) {
						camera.Rotate({ 0,0,Math::ToRadians(15.f) });
					}
				}
				if (e->GetID() == WindowEventID::KeyUp) {
					auto& procEvnt = static_cast<WindowEvent_KeyUp&>(*e);
					std::cout << window->GetName() << " " << procEvnt.GetEventName() << " " << procEvnt.mCode.ToString() << std::endl;
				}
				if (e->GetID() == WindowEventID::MouseButtonDown) {
					auto& procEvnt = static_cast<WindowEvent_MouseDown&>(*e);
					std::cout << window->GetName() << procEvnt.GetEventName() << " " << procEvnt.mCode.ToString() << std::endl;
				}
				else if (e->GetID() == WindowEventID::TextChar) {
					auto& procEvnt = static_cast<WindowEvent_TextChar&>(*e);
					ZE_LOG("{}", procEvnt.mCharacter);
					//std::cout << procEvnt.GetEventName() << " " << procEvnt.mCharacter << std::endl;
				}
				else if (e->GetID() == WindowEventID::MouseEnter) {
					auto& procEvnt = static_cast<WindowEvent_MouseEnter&>(*e);
					std::cout << procEvnt.GetEventName() << std::endl;
				}
				else if (e->GetID() == WindowEventID::MouseExit) {
					auto& procEvnt = static_cast<WindowEvent_MouseExit&>(*e);
					std::cout << procEvnt.GetEventName() << std::endl;
				}
			}
			window->EndFrame();
			
			SwapChain* swapChain = swapChainList[k].get();
			RenderTarget* target = swapChain->GetRenderTarget();
			const Vec2i windowSize = swapChain->GetWindowSize();
			camera.SetAspectRatio(static_cast<float>(windowSize.X) / static_cast<float>(windowSize.Y));
			//camera.LookAt({ 0,0,0 });
			dataCB.mMatrix = camera.GetProjectionMatrix() * camera.GetViewMatrix() * worldMatrix;
			dataCB.mMatrix = Math::Transpose(dataCB.mMatrix);
			ctx->UpdateBuffer(*constantBuffer, &dataCB, sizeof(dataCB));

			ctx->SetPrimitiveTopology(PrimitiveTopology::TriangleList);
			ctx->SetRenderTarget(target);
			ctx->Clear(Color{ .3f,0,0.f });

			ctx->SetShader(shader.get());
			ctx->SetTexture(texture.get());

			ctx->SetVertexBuffer(*vertexBuffer);
			ctx->SetIndexBuffer(*indexBuffer);
			ctx->SetConstantBuffer(*constantBuffer);

			ctx->DrawIndexed(indexBuffer->GetSize(), 0);

			swapChain->SwapBuffers();

		}

	}
}

int main(int argc, char** argv) {

	TestWindow();

	Mat4 test;
	test[1][3] = 1;
	std::cout << test[1][3];

	return 0;
}