// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Window/WindowContext.h"

namespace ZeronEngine
{
	/*
	 * Window module creates and manages window contexes of specified type
	 */
	class WindowModule
	{
	public:
		WindowModule();
		~WindowModule();
		
		/*
		 * Create a window context with the defined parameters
		 */
		template<typename T>
		WindowContextHandle CreateWindowContext(WindowProps&& windowProps)
		{
			// Initial configurations for the context that will be used by all windows
			if(T::ConfigureContext())
			{
				// Set event dispatcher as module dispatcher if it is not explicitly defined
				windowProps.EventDispatcher = m_EventDispatcher && !windowProps.EventDispatcher ?
					m_EventDispatcher : windowProps.EventDispatcher;
				// Create window handle
				WindowContextHandle handle(m_WindowCounter, this);
				windowProps.ContextHandle = handle;
				// Update window counter
				m_WindowCounter++;
				
				// Create and initialize window context and contain the reference
				auto windowContext = std::unique_ptr<WindowContext>(new T(std::move(windowProps)));
				windowContext->RegisterEvents();
				m_WindowContextContainer[handle.GetHandleID()] = std::move(windowContext);
				return handle;
			}

			return WindowContextHandle();
		}

		void Init();
		void Update();
		void Destroy();

		/* Register to dispatcher and cache it as reference*/
		void RegisterEvents(const EventDispatcher& Dispatcher);

		bool HasWindow(const WindowContextHandle& Handle) const;

		// Get window pointer with window id
		WindowContext* GetMainWindow() const;
		
		// Get window pointer with window id
		WindowContext* GetWindow(int windowID) const;
		
		// Get window pointer with window handle
		WindowContext* GetWindow(const WindowContextHandle& handle) const;

		/* Return the count of windows managed by this module */
		int GetWindowCount() const { return static_cast<int>(m_WindowContextContainer.size()); }
		
		/* Schedule a window for removal if it exists*/
		bool RemoveWindow(const WindowContextHandle& contextHandle);

		bool RemoveAll();

	private:
		// list of windows that are spawned by the module with unique ids
		std::unordered_map<int, std::unique_ptr<WindowContext>> m_WindowContextContainer;

		// list of window ids that are scheduled to be removed
		std::queue<int> m_WindowRemoveContainer;
		
		// Dispatcher reference that is module bound to
		const EventDispatcher* m_EventDispatcher;

		// Handle to the main window that will be created by the module
		WindowContextHandle m_MainWindowHandle;

		// Window counter for giving windows unique IDs
		int m_WindowCounter;
	};

}
