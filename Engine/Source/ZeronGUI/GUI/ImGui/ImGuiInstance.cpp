// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <GUI/ImGui/ImGuiInstance.h>

#include <Graphics/Graphics.h>
#include <GUI/ImGui/ImGuiRenderer.h>
#include <imgui/imgui_internal.h>
#include <Platform/Window.h>

namespace Zeron
{
	class GraphicsD3D11;

	ImGuiInstance::ImGuiInstance()
		: mContext(ImGui::CreateContext())
	{}

	ImGuiInstance::~ImGuiInstance()
	{
		mImGuiRenderer->Destroy(*mContext);
		ImGui::DestroyContext(mContext);
	}

	bool ImGuiInstance::Init(Gfx::Graphics& graphics, Gfx::GraphicsContext& graphicsContext, std::shared_ptr<Gfx::ShaderProgram> shaderProgram)
	{
		ImGuiIO& io = mContext->IO;
		// io.FontGlobalScale = io.FontGlobalScale * 2;
		io.BackendPlatformName = "Zeron Engine - ImGui";
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.KeyMap[ImGuiKey_Tab] = KeyCode::Tab;
		io.KeyMap[ImGuiKey_LeftArrow] = KeyCode::Left;
		io.KeyMap[ImGuiKey_RightArrow] = KeyCode::Right;
		io.KeyMap[ImGuiKey_UpArrow] = KeyCode::Up;
		io.KeyMap[ImGuiKey_DownArrow] = KeyCode::Down;
		io.KeyMap[ImGuiKey_PageUp] = KeyCode::PageUp;
		io.KeyMap[ImGuiKey_PageDown] = KeyCode::PageDown;
		io.KeyMap[ImGuiKey_Home] = KeyCode::Home;
		io.KeyMap[ImGuiKey_End] = KeyCode::End;
		io.KeyMap[ImGuiKey_Insert] = KeyCode::Insert;
		io.KeyMap[ImGuiKey_Delete] = KeyCode::Delete;
		io.KeyMap[ImGuiKey_Backspace] = KeyCode::Backspace;
		io.KeyMap[ImGuiKey_Space] = KeyCode::Space;
		io.KeyMap[ImGuiKey_Enter] = KeyCode::Enter;
		io.KeyMap[ImGuiKey_Escape] = KeyCode::Escape;
		io.KeyMap[ImGuiKey_KeyPadEnter] = KeyCode::KPEnter;
		io.KeyMap[ImGuiKey_A] = KeyCode::A;
		io.KeyMap[ImGuiKey_C] = KeyCode::C;
		io.KeyMap[ImGuiKey_V] = KeyCode::V;
		io.KeyMap[ImGuiKey_X] = KeyCode::X;
		io.KeyMap[ImGuiKey_Y] = KeyCode::Y;
		io.KeyMap[ImGuiKey_Z] = KeyCode::Z;

		_applyStyle();

		mImGuiRenderer = std::make_unique<ImGuiRenderer>(*mContext, graphics, graphicsContext, shaderProgram);
		return true;
	}

	void ImGuiInstance::NewFrame()
	{
		const auto now = std::chrono::high_resolution_clock::now();
		mContext->IO.DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - mTime).count();
		mTime = now;
		mContext->IO.DisplaySize = mImGuiRenderer->GetDisplaySize();
		mContext->IO.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		mImGuiRenderer->NewFrame(*mContext);
		ImGui::NewFrame();
	}

	void ImGuiInstance::Update(Gfx::Graphics& graphics)
	{
		// Render all ImGui commands
		ImGui::Render();
		// Update render resources
		mImGuiRenderer->Update(*mContext, graphics);
	}

	void ImGuiInstance::Draw(Gfx::CommandBuffer& cmd) const
	{
		mImGuiRenderer->Draw(*mContext, cmd);
	}

	bool ImGuiInstance::HandleEvent(const SystemEvent& evt) const
	{
		bool handled = false;
		ImGuiIO& io = mContext->IO;
		std::visit(
			TypeTraits::Visitor{
				[&](const SystemEvent::WindowFocused&) {
					io.AddFocusEvent(true);
				},
				[&](const SystemEvent::WindowUnfocused&) {
					io.AddFocusEvent(false);
				},
				[&](const SystemEvent::KeyDown& data) {
					if (io.WantCaptureKeyboard) {
						handled = _onKey(data.mCode, true);
					}
				},
				[&](const SystemEvent::KeyUp& data) {
					if (io.WantCaptureKeyboard) {
						handled = _onKey(data.mCode, false);
					}
				},
				[&](const SystemEvent::TextChar& data) {
					if (io.WantTextInput) {
						io.AddInputCharacter(data.mCharacter);
						handled = true;
					}
				},
				[&](const SystemEvent::MouseButtonDown& data) {
					if (const int mouseButton = data.mCode; mouseButton < static_cast<int>(std::size(io.MouseDown))) {
						io.MouseDown[mouseButton] = true;
						io.MousePos = ImVec2(static_cast<float>(data.mPosX), static_cast<float>(data.mPosY));
						if (mContext->HoveredWindow) {
							handled = true;
						}
					}
				},
				[&](const SystemEvent::MouseButtonUp& data) {
					if (const int mouseButton = data.mCode; mouseButton < static_cast<int>(std::size(io.MouseDown))) {
						io.MouseDown[mouseButton] = false;
						if (mContext->HoveredWindow) {
							handled = true;
						}
					}
				},
				[&](const SystemEvent::MouseScroll& data) {
					if (mContext->HoveredWindow) {
						io.MouseWheel += data.mOffsetY;
						handled = true;
					}
				},
				[&](const SystemEvent::MouseMoved& data) {
					io.MousePos = ImVec2(static_cast<float>(data.mPosX), static_cast<float>(data.mPosY));
				},
				[](const auto&) {
					return;
				},
			},
			evt.GetData()
		);

		return handled;
	}

	bool ImGuiInstance::_onKey(KeyCode::Type code, bool isPressed) const
	{
		ImGuiIO& io = mContext->IO;
		switch (code) {
			case KeyCode::LeftControl:
			case KeyCode::RightControl: {
				io.KeyCtrl = isPressed;
			} break;
			case KeyCode::LeftAlt:
			case KeyCode::RightAlt: {
				io.KeyAlt = isPressed;
			} break;
			case KeyCode::LeftShift:
			case KeyCode::RightShift: {
				io.KeyShift = isPressed;
			} break;
			case KeyCode::LeftSuper:
			case KeyCode::RightSuper: {
				io.KeySuper = isPressed;
			} break;
			default:;
		}
		io.KeysDown[code] = isPressed;
		return true;
	}

	void ImGuiInstance::_applyStyle()
	{
		ImGuiStyle& style = mContext->Style;
		style.WindowRounding = 4.0f;
		style.FrameRounding = 4.0f;
		style.PopupRounding = 4.0f;
		style.ScrollbarRounding = 4.0f;
		style.GrabRounding = 4.0f;
		style.FramePadding = ImVec2(6.0f, 4.0f);
		style.ItemSpacing = ImVec2(10.0f, 8.0f);
		style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
		style.IndentSpacing = 20.0f;
		style.ScrollbarSize = 15.0f;

		ImVec4* colors = style.Colors;
		colors[ImGuiCol_Text] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	}
}