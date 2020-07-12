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

				// Create and initialize window context and contain the reference
				auto windowContext = std::unique_ptr<WindowContext>(new T(std::move(windowProps)));
				if(windowContext)
				{
					windowContext->RegisterEvents();
					WindowContextHandle handle(*windowContext);
					m_WindowContextContainer[handle] = std::move(windowContext);
					return handle;
				}
			}

			return WindowContextHandle();
		}

		void Init();
		void Update();
		void Destroy();

		/* Register to dispatcher and cache it as reference*/
		void RegisterEvents(const EventDispatcher& Dispatcher);

		WindowContext* GetWindow(const WindowContextHandle& Handle) const;
		
		/* Schedule a window for removal if it exists*/
		bool RemoveWindow(const WindowContextHandle& contextHandle);
		
		/* Return the count of windows managed by this module */
		int GetWindowCount() const { return static_cast<int>(m_WindowContextContainer.size()); }

	private:

		// list of windows that are spawned by the container
		std::unordered_map<WindowContextHandle, std::unique_ptr<WindowContext>> m_WindowContextContainer;

		// list of windows that are scheduled to be removed
		std::queue<WindowContextHandle> m_WindowRemoveContainer;
		
		// Dispatcher reference that is module bound to
		const EventDispatcher* m_EventDispatcher;

	};

}
