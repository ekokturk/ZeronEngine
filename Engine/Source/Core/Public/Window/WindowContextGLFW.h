// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "WindowContext.h"

struct GLFWwindow;
class EventDispatcher;

namespace ZeronEngine
{
	/*
	 * Window wrapper for GLFW library
	 */

	class WindowContextGLFW : public WindowContext
	{
	public:

		// TODO
		// SetInputMode
		//virtual void SetIcon() = 0;
		//virtual void SetPosition() = 0;
		// virtual Vec2 GetPosition() = 0;
		// GetFramebufferSize
		// GetContentScale
		// https://www.glfw.org/docs/3.3/group__input.html#ga571e45a030ae4061f746ed56cb76aede
		
		/* Initialize window dependencies if not initialized yet */
		static bool ConfigureContext();
		
		WindowContextGLFW(WindowProps&& windowProps);
		~WindowContextGLFW();

		virtual void Update() override;
		virtual void Destroy() override;
		virtual void RegisterEvents() override;

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
		virtual void SetMousePosition(const Vector2& mousePosition) override;
		

	private:
		GLFWwindow* m_WindowHandle;
		static int s_WindowCount;
	};

}
