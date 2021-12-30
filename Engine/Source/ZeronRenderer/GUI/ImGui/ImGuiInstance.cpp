// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "ImGuiInstance.h"
#include "Graphics/Graphics.h"
#include "Window/Window.h"
#include "imgui/imgui_internal.h"

namespace Zeron
{
	class GraphicsD3D11;

	ImGuiInstance::ImGuiInstance()
		: mContext(ImGui::CreateContext())
		, mWindow(nullptr)
	{
	}

	ImGuiInstance::~ImGuiInstance()
	{
		mImGuiGraphics.Destroy(*mContext);
		ImGui::DestroyContext(mContext);
	}

	bool ImGuiInstance::Init(Graphics& graphics, Window& window)
	{
		mWindow = &window;
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

		return mWindow && 
			mImGuiGraphics.Init(*mContext, graphics);
	}

	void ImGuiInstance::NewFrame()
	{
		const auto now = std::chrono::high_resolution_clock::now();
		mContext->IO.DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - mTime).count();
		mTime = now;
		mImGuiGraphics.NewFrame(*mContext);
		if (mWindow) {
			mContext->IO.DisplaySize = ImVec2(static_cast<float>(mWindow->GetSize().X),
				static_cast<float>(mWindow->GetSize().Y));
		}
		ImGui::NewFrame();
	}

	void ImGuiInstance::Draw() const
	{
		ImGui::Render();
		mImGuiGraphics.Draw(*mContext);
	}

	bool ImGuiInstance::HandleEvent(WindowEvent& evt) const
	{
		bool handled = false;
		ImGuiIO& io = mContext->IO;
		switch (evt.GetID()) {
			case WindowEventID::WindowClosed: break;
			case WindowEventID::WindowCreated: break;
			case WindowEventID::WindowFocused: {
				io.AddFocusEvent(true);
			} break;
			case WindowEventID::WindowUnfocused: {
				io.AddFocusEvent(false);
			} break;
			case WindowEventID::WindowResized: {
				
			} break;
			case WindowEventID::WindowMoved: break;
			case WindowEventID::WindowMinimized: break;
			case WindowEventID::WindowMaximized: break;
			case WindowEventID::WindowRestored: break;
			case WindowEventID::KeyDown: {
				if(io.WantCaptureKeyboard) {
					auto& keyDownEvt = static_cast<WindowEvent_KeyDown&>(evt);
					handled = OnKey_(keyDownEvt.mCode, true);
				}
			} break;
			case WindowEventID::KeyUp: {
				if (io.WantCaptureKeyboard) {
					auto& keyUpEvt = static_cast<WindowEvent_KeyUp&>(evt);
					handled = OnKey_(keyUpEvt.mCode, false);
				}
			} break;
			case WindowEventID::TextChar: {
				if(io.WantTextInput) {
					auto& charEvt = static_cast<WindowEvent_TextChar&>(evt);
					io.AddInputCharacter(charEvt.mCharacter);
					handled = true;
				}
			} break;
			case WindowEventID::MouseEnter: break;
			case WindowEventID::MouseExit: break;
			case WindowEventID::MouseButtonDown: {
				auto& mouseDownEvt = static_cast<WindowEvent_MouseDown&>(evt);
				if (const int mouseButton = mouseDownEvt.mCode;
					mouseButton < static_cast<int>(std::size(io.MouseDown))) {
					io.MouseDown[mouseButton] = true;
					if (mContext->HoveredWindow) {
						handled = true;
					}
				}
			} break;
			case WindowEventID::MouseButtonUp: {
				auto& mouseUpEvt = static_cast<WindowEvent_MouseUp&>(evt);
				if (const int mouseButton = mouseUpEvt.mCode;
					mouseButton < static_cast<int>(std::size(io.MouseDown))) {
					io.MouseDown[mouseButton] = false;
					if(mContext->HoveredWindow) {
						handled = true;
					}
				}
			} break;
			case WindowEventID::MouseScroll: {
				if (mContext->HoveredWindow) {
					auto& scrollEvt = static_cast<WindowEvent_MouseScrolled&>(evt);
					io.MouseWheel += scrollEvt.mOffsetY;
					handled = true;
				}
			} break;
			case WindowEventID::MouseMoved: {
				auto& movedEvt = static_cast<WindowEvent_MouseMoved&>(evt);
				io.MousePos = ImVec2(static_cast<float>(movedEvt.mPosX),
					static_cast<float>(movedEvt.mPosY));
			} break;
			default: ;
		}
		return handled;
	}

	bool ImGuiInstance::OnKey_(KeyCode::Type code, bool isPressed) const
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
