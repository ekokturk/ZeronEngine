// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Window/Window.h>

class EventDispatcher;
struct GLFWcursor;
struct GLFWmonitor;
struct GLFWwindow;

namespace Zeron {
	
	class WindowGLFW final : public Window {
	public:
		WindowGLFW(const WindowConfig& windowProps);
		~WindowGLFW();

		virtual bool Init() override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void SetVisible() override;
		virtual void SetHidden() override;
		
		virtual void SetMinimized() override;
		virtual void SetMaximized() override;
		virtual void SetRestored() override;

		virtual void SetFocused() override;
		virtual void SetAttention() override;
		
		virtual void SetName(const std::string& name) override;
		virtual void SetAspectRatio(int numerator, int denominator) override;
		virtual void SetSize(int width, int height) override;
		virtual void SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight) override;
		virtual void SetScreenPosition(int posX, int posY) override;

		virtual void SetClipCursor(bool shouldClip) override;

		virtual void* GetAPIHandle() const override;
		virtual void* GetPlatformHandle() const override;
		
		// This is needed for input callbacks
		void QueueEvent(std::unique_ptr<WindowEvent> e);
		bool IsCursorClipped() const { return mIsCursorClipped; }

		GLFWmonitor* FindCurrentMonitor() const;

	private:
		void RegisterEvents_();
		virtual void OnFullScreenChangedBorderless_() override;
		virtual void OnFullScreenChangedMonitor_() override;

		// Native GLFW window is set to point to WindowGLFW
		static WindowGLFW* GetUserPointerGLFW_(GLFWwindow* windowGLFW);

		static KeyCode GetKeyCodeGLFW_(int code);
		static MouseCode GetMouseCodeGLFW_(int code);
		
		GLFWwindow* mWindowGLFW;
		GLFWmonitor* mMonitorGLFW;
		GLFWcursor* mCursorGLFW;

		bool mIsResizing;
		bool mIsCursorClipped;

		static int mWindowGLFWCount;
	};

}
