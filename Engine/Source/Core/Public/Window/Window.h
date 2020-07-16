// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "CoreTypes/CoreTypes.h"
#include "Window/WindowHandle.h"

/*
 * Window abstract class 
 */

namespace ZeronEngine
{
	class  EventDispatcher;
	class  Window;
	class  WindowModule;
	
	/*
	 * Properties for Window creation
	 * They are required for Window initialization and they are updated on change
	 */
	
	struct WindowProps
	{
		WindowProps(const EventDispatcher* dispatcher = nullptr);
		WindowProps(std::string& name, int width, int height, const EventDispatcher* dispatcher);
		
		std::string Name;
		int Width;
		int Height;
		bool IsFullScreen;
		int RefreshRate;
		Vector2 MousePosition;
		WindowHandle ContextHandle;
		const EventDispatcher* EventDispatcher;

		int WidthPrev;
		int HeightPrev;
	};
	
	class Window 
	{
	public:
	
		/*
		 * Initialize context for the window that will be created
		 * Library method calls and initializations for all windows of context type can be done here
		 */
		template<typename T>
		static void InitContext();

		virtual void Update() = 0;
		virtual void Destroy() = 0;

		// Register window events that will be dispatched
		virtual void RegisterEvents() = 0;

		// Make window visible if it was hidden
		virtual void SetVisible() = 0;
		// Make window hidden if it was visible
		virtual void SetHidden() = 0;

		// Minimize window 
		virtual void SetMinimized() = 0;
		// Maximize window 
		virtual void SetMaximized() = 0;
		// Restore window to prior state (ex. from maximized/minimized) 
		virtual void SetRestored() = 0;

		// Make window current focus
		virtual void SetFocused() = 0;

		// Request attention to the window
		virtual void SetAttention() = 0;
		
		// Restore window name 
		virtual void SetName(const std::string& name) = 0;
		// Set size of the window in terms of pixels
		virtual void SetSize(int width, int height) = 0;
		// Set size limits of the window
		virtual void SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight) = 0;
		// Restore window aspect ratio (ex: 16:9 -> numerator:16, denominator:9
		virtual void SetAspectRatio(int numerator, int denominator) = 0;
		// Set position of the mouse cursor in window
		virtual void SetMousePosition(const Vector2& mousePosition) = 0;

		virtual void SetFullScreen(bool isFullScreen) = 0;

		
		int GetWidth() const									{ return m_WindowProps.Width; }
		int GetHeight() const									{ return m_WindowProps.Height; }
		const std::string& GetName() const						{ return m_WindowProps.Name; }
		Vector2 GetMousePosition() const						{ return m_WindowProps.MousePosition; }
		bool IsFullScreen() const								{ return m_WindowProps.IsFullScreen; }

		WindowHandle GetWindowHandle() const		{ return m_WindowProps.ContextHandle; }


	public:
		virtual ~Window();
		
	protected:
		Window(const WindowProps& windowProps);
		Window(WindowProps&& windowProps);

		
	protected:
		WindowProps m_WindowProps;
	};

}

