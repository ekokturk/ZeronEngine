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
#include "Renderer/Model.h"
#include "GUI/ImGui/ImGuiInstance.h"

using namespace Zeron;

struct VertexShaderCBData
{
	Mat4 mMatrix;
};

struct PixelShaderCBData
{
	//float mAlpha = 1.f;
	Vec3 mAmbientLightColor = Vec3::ONE;
	float mAmbientLightStrength = 1.f;
};

struct WindowContext {
	WindowContext(std::unique_ptr<Window> window) : mWindow(move(window)) {}
	
	std::unique_ptr<Window> mWindow;
	std::unique_ptr<ImGuiInstance> mImGui;
	std::shared_ptr<SwapChain> mSwapChain;
};

void TestWindow()
{
	auto gfx = std::make_unique<GraphicsD3D11>();
	if (!gfx->Init()) {
		return;
	}
	auto ctx = gfx->GetImmediateContext();
	
	std::vector<WindowContext> windowCtxList;

	windowCtxList.emplace_back(WindowContext(Window::CreatePlatformWindow(WindowAPI::SDL, WindowConfig("SDL", 800, 600, 0))));
	windowCtxList.emplace_back(WindowContext(Window::CreatePlatformWindow(WindowAPI::Win32, WindowConfig("Win32", 800, 600, 0))));
	windowCtxList.emplace_back(WindowContext(Window::CreatePlatformWindow(WindowAPI::GLFW, WindowConfig("GLFW", 800, 600, 0))));


	for (auto& window : windowCtxList) {
		window.mWindow->Init();
		window.mSwapChain = gfx->CreateSwapChain(*window.mWindow);
		window.mSwapChain->SetVSyncEnabled(true);
		window.mImGui = std::make_unique<ImGuiInstance>();
		window.mImGui->Init(*gfx, *window.mWindow);
	}



	VertexShaderCBData vertexCB;
	PixelShaderCBData pixelCB;
	Camera camera;
	camera.SetPosition({ 0.f, 300, -300.f });
	camera.SetFieldOfView(60.f);


	auto constantBufferVS = gfx->CreateConstantBuffer<VertexShaderCBData>(vertexCB);
	auto constantBufferPS = gfx->CreateConstantBuffer<PixelShaderCBData>(pixelCB);
	auto shader = gfx->CreateShader("Default");
	auto modelTexture = gfx->CreateTexture(TextureType::Diffuse, "Resources/Textures/TestHumanoid_CLR.png");

	Model model(*gfx, "Resources/Models/TestHumanoid_Model.fbx", constantBufferVS);
	for(auto& mesh : model.GetMeshes()) {
		mesh.AddTexture(modelTexture);
	}
	
	Mat4 worldMatrix;

	char buffText[256] = {};

	bool isResizing = false;
	float cameraSensitivity = 1.f;
	bool isCameraLookAtEnabled = true;
	
	bool isRunning = true;
	while (isRunning) {
		for (auto& windowCtx : windowCtxList) {
			Window* window = windowCtx.mWindow.get();
			SwapChain* swapChain = windowCtx.mSwapChain.get();
			ImGuiInstance& imgui = *windowCtx.mImGui;

			window->BeginFrame();
			imgui.NewFrame();

			while (auto e = window->GetNextEvent()) {
				if(imgui.HandleEvent(*e)) {
					continue;
				}
				
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
						camera.Move(camera.GetForwardDir() * -cameraSensitivity);
					}
					if (procEvnt.mCode == KeyCode::S) {
						camera.Move(camera.GetForwardDir() * cameraSensitivity);
					}
					if (procEvnt.mCode == KeyCode::A) {
						camera.Move(camera.GetRightDir() * -cameraSensitivity);
					}
					if (procEvnt.mCode == KeyCode::D) {
						camera.Move(camera.GetRightDir() * cameraSensitivity);
					}
					if (procEvnt.mCode == KeyCode::Q) {
						camera.Move(camera.GetUpDir() * -cameraSensitivity);
					}
					if (procEvnt.mCode == KeyCode::E) {
						camera.Move(camera.GetUpDir() * cameraSensitivity);
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
				else if (e->GetID() == WindowEventID::WindowResized) {
					auto& procEvnt = static_cast<WindowEvent_WindowResized&>(*e);
					ctx->ResizeSwapChain(*swapChain, { procEvnt.mWidth, procEvnt.mHeight });
					swapChain->SetViewport({}, { procEvnt.mWidth, procEvnt.mHeight });
				}
			}
			window->EndFrame();

			ImGui::Begin("Debug Window");
			ImGui::SliderFloat3("Ambient Light Color", reinterpret_cast<float*>(&pixelCB.mAmbientLightColor), 0.f, 1.f);
			ImGui::SliderFloat("Ambient Light Strength", &pixelCB.mAmbientLightStrength, 0.f, 1.f);
			float rotation[] = { camera.GetRotation().X, camera.GetRotation().Y, camera.GetRotation().Z };
			if(ImGui::SliderFloat3("Rotation", rotation, -Math::PI<float>(), Math::PI<float>())) {
				camera.SetRotation({ rotation[0], rotation[1], rotation[2] });
			}
			float position[] = { camera.GetPosition().X, camera.GetPosition().Y, camera.GetPosition().Z };
			if (ImGui::DragFloat3("Position", position, -20, 20)) {
				camera.SetPosition({ position[0], position[1], position[2] });
			}
			ImGui::Checkbox("Camera Look At", &isCameraLookAtEnabled);
			ImGui::End();

			RenderTarget* target = swapChain->GetRenderTarget();
			ctx->SetPrimitiveTopology(PrimitiveTopology::TriangleList);
			ctx->SetRenderTarget(target);
			ctx->Clear(Color{ .3f,0,0.f });

			const Vec2i windowSize = swapChain->GetWindowSize();
			camera.SetAspectRatio(static_cast<float>(windowSize.X) / static_cast<float>(windowSize.Y));
			if(isCameraLookAtEnabled) {
				camera.LookAt({ 0,0,0 });
			}

			{
				ctx->UpdateBuffer(*constantBufferPS, &pixelCB, sizeof(pixelCB));
				ctx->SetConstantBuffer(*constantBufferPS, ShaderType::Fragment);
				ctx->SetShader(shader.get());
				model.Draw(*ctx, camera);
			}

			imgui.Draw();

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