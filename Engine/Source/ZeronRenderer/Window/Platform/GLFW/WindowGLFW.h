// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Window/Window.h"

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWcursor;
class EventDispatcher;

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

		virtual void* GetPlatformHandle() const override;
		
		// This is needed for input callbacks
		void QueueEvent(std::unique_ptr<WindowEvent> e);
		bool IsCursorClipped() const { return mIsCursorClipped; }

		GLFWmonitor* FindCurrentMonitor() const;

	private:
		void RegisterEvents();
		virtual void OnFullScreenChangedBorderless() override;
		virtual void OnFullScreenChangedMonitor() override;

		// Native GLFW window is set to point to WindowGLFW
		static WindowGLFW* GetUserPointerGLFW(GLFWwindow* windowGLFW);

		static KeyCode GetKeyCodeGLFW(int code);
		static MouseCode GetMouseCodeGLFW(int code);
		
		GLFWwindow* mWindowGLFW;
		GLFWmonitor* mMonitorGLFW;
		GLFWcursor* mCursorGLFW;

		bool mIsCursorClipped;

		static int mWindowGLFWCount;
	};

}
