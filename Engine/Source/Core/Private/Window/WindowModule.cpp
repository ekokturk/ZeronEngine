// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/WindowModule.h"
#include "Events/EventDispatcher.h"
#include "Events/EventTypes/EventTypes.h"
#include "Window/WindowContextGLFW.h"

namespace ZeronEngine
{
	WindowModule::WindowModule()
		:m_EventDispatcher(nullptr),
		m_WindowCounter(0)
	{
	}

	WindowModule::~WindowModule()
	{
	}

	void WindowModule::Init()
	{
		// Create main window
		m_MainWindowHandle = CreateWindowContext<WindowContextGLFW>(WindowProps(m_EventDispatcher));
		
		CreateWindowContext<WindowContextGLFW>(WindowProps(m_EventDispatcher));
	}

	void WindowModule::Destroy()
	{

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
			// Dispatch event for main window closure
			if(m_MainWindowHandle.GetHandleID() == m_WindowRemoveContainer.front())
			{
				m_EventDispatcher->Dispatch<Events::Window::MainClose>(Events::Window::MainClose());
			}
			m_WindowContextContainer.erase(m_WindowRemoveContainer.front());
			m_WindowRemoveContainer.pop();

			// Send all windows closed event
			//if (m_WindowContextContainer.empty()){	}
		}
	}
	
	bool WindowModule::HasWindow(const WindowContextHandle& handle) const
	{
		return handle.m_WindowCreator == this && m_WindowContextContainer.count(handle.GetHandleID()) > 0;
	}

	WindowContext* WindowModule::GetWindow(int windowID) const
	{
		if(m_WindowContextContainer.count(windowID) > 0)
		{
			return m_WindowContextContainer.at(windowID).get();
		}

		return nullptr;
	}

	WindowContext* WindowModule::GetWindow(const WindowContextHandle& handle) const
	{
		if (HasWindow(handle))
		{
			return m_WindowContextContainer.at(handle.GetHandleID()).get();
		}

		return nullptr;
	}
	
	bool WindowModule::RemoveWindow(const WindowContextHandle& handle)
	{
		if (HasWindow(handle))
		{
			m_WindowRemoveContainer.push(handle.GetHandleID());
			return true;
		}

		return false;
	}


	void WindowModule::RegisterEvents(const EventDispatcher& Dispatcher)
	{
		m_EventDispatcher = &Dispatcher;

		// WINDOW CLOSED
		Dispatcher.Register<Events::Window::Close>(this, [=](const Events::Window::Close& e)
		{
			e.Consume();
			RemoveWindow(e.ContextHandle); // Schedule to remove
		});
	}


}
