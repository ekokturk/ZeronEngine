// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/WindowModule.h"


#include "Application.h"
#include "Events/EventDispatcher.h"
#include "Events/EventTypes.h"
#include "Window/WindowContextGLFW.h"

namespace ZeronEngine
{
	WindowModule::WindowModule()
		:m_EventDispatcher(nullptr)
	{
	}

	WindowModule::~WindowModule()
	{
	}

	void WindowModule::Init()
	{
		CreateWindowContext<WindowContextGLFW>(WindowProps(m_EventDispatcher));
		CreateWindowContext<WindowContextGLFW>(WindowProps(m_EventDispatcher));
		CreateWindowContext<WindowContextGLFW>(WindowProps(m_EventDispatcher));
		CreateWindowContext<WindowContextGLFW>(WindowProps(m_EventDispatcher));
	}

	void WindowModule::Update()
	{
		// Update all windows
		for(const auto& window : m_WindowContextContainer )
		{
			// Update every window that is managed by this module
			window.second->Update();
		}

		// Delete windows that are scheduled to be removed
		while(m_WindowRemoveContainer.empty() == false)
		{
			// Remove window
			ZERON_LOG_INFO("Window '{}' is closed...", m_WindowRemoveContainer.front().WindowRef->GetName())
			m_WindowContextContainer.erase(m_WindowRemoveContainer.front());
			m_WindowRemoveContainer.pop();

			// Send all windows closed event
			if (m_WindowContextContainer.empty())
			{
				m_EventDispatcher->Dispatch<Events::Window_All_Closed>(Events::Window_All_Closed());
			}
		}
	}

	void WindowModule::Destroy()
	{
		
	}

	bool WindowModule::RemoveWindow(const WindowContextHandle& contextHandle)
	{
		if (m_WindowContextContainer.count(contextHandle))
		{
			m_WindowRemoveContainer.push(contextHandle);
			return true;
		}

		return false;
	}
	
	void WindowModule::RegisterEvents(const EventDispatcher& Dispatcher)
	{
		m_EventDispatcher = &Dispatcher;
		
		Dispatcher.Register<Events::Window_Closed>(this, [=](const Events::Window_Closed& e)
		{
			e.Consume();
			RemoveWindow(e.ContextHandle); // Schedule to remove
		});
	}



}
