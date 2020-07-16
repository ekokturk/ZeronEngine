// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Logger.h"
#include "Window/Window.h"

namespace ZeronEngine
{
	/*
	 * Window module creates and manages window contexes of specified type
	 * Main window will have the index of 0
	 */
	class WindowModule
	{
	public:
		WindowModule();
		~WindowModule();
		
		/*
		 * Create a Window with the defined parameters
		 */
		template<typename T>
		WindowHandle CreateNewWindow(WindowProps&& windowProps)
		{
			if (m_WindowCounter > 0) {
				ZERON_LOG_WARNING("Window creation failed! Multi-Window creation is not yet supported.")
				return WindowHandle();
			}
			
			// Initial configurations for the context that will be used by all windows
			if(T::ConfigureContext())
			{
				// Set event dispatcher as module dispatcher if it is not explicitly defined
				windowProps.EventDispatcher = m_EventDispatcher;
				// Create window handle
				WindowHandle handle(m_WindowCounter, this);
				windowProps.ContextHandle = handle;
				// Update window counter
				
				// Create and initialize Window and contain the reference
				auto window = std::unique_ptr<Window>(new T(std::move(windowProps)));
				window->RegisterEvents();

				m_WindowContainer[handle.GetHandleID()] = std::move(window);

				m_WindowCounter++;	// Increment window 
				return handle;
			}

			return WindowHandle();
		}

		void Init();
		void Update();
		void Destroy();

		/* Register to dispatcher and cache it as reference*/
		void RegisterEvents(const EventDispatcher& Dispatcher);

		bool HasWindow(const WindowHandle& Handle) const;

		// Get window pointer with window id
		Window* GetMainWindow() const;
		
		// Get window pointer with window id
		Window* GetWindow(int windowID) const;
		
		// Get window pointer with window handle
		Window* GetWindow(const WindowHandle& handle) const;

		/* Return the count of windows managed by this module */
		int GetWindowCount() const { return static_cast<int>(m_WindowContainer.size()); }
		
		/* Schedule a window for removal if it exists*/
		bool RemoveWindow(const WindowHandle& contextHandle);

		bool RemoveAll();

	private:
		// list of windows that are spawned by the module with unique ids
		std::unordered_map<int, std::unique_ptr<Window>> m_WindowContainer;

		// list of window ids that are scheduled to be removed
		std::queue<int> m_WindowRemoveContainer;
		
		// Dispatcher reference that is module bound to
		const EventDispatcher* m_EventDispatcher;

		// Window counter for giving windows unique IDs (Main window 0)
		int m_WindowCounter;
	};

}
