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
	{
	}

	ImGuiInstance::~ImGuiInstance()
	{
		mImGuiRenderer->Destroy(*mContext);
		ImGui::DestroyContext(mContext);
	}

	bool ImGuiInstance::Init(Graphics& graphics, GraphicsContext& graphicsContext)
	{
		ImGuiIO& io = mContext->IO;
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

		mImGuiRenderer = std::make_unique<ImGuiRenderer>();
		return mImGuiRenderer->Init(*mContext, graphics, graphicsContext);
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

	void ImGuiInstance::Update(Graphics& graphics)
	{
		// Render all ImGui commands
		ImGui::Render();
		// Update render resources
		mImGuiRenderer->Update(*mContext, graphics);
	}

	void ImGuiInstance::Draw(CommandBuffer& cmd) const
	{
		mImGuiRenderer->Draw(*mContext, cmd);
	}

	bool ImGuiInstance::HandleEvent(const SystemEvent& evt) const
	{
		bool handled = false;
		ImGuiIO& io = mContext->IO;
		std::visit(Visitor{
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
				if (const int mouseButton = data.mCode;
					mouseButton < static_cast<int>(std::size(io.MouseDown))) {
					io.MouseDown[mouseButton] = true;
					if (mContext->HoveredWindow) {
						handled = true;
					}
				}
			},
			[&](const SystemEvent::MouseButtonUp& data) {
				if (const int mouseButton = data.mCode;
					mouseButton < static_cast<int>(std::size(io.MouseDown))) {
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
				io.MousePos = ImVec2(static_cast<float>(data.mPosX),static_cast<float>(data.mPosY));
			},
			[](const auto&) { return; },
		}, evt.GetData());

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
}
