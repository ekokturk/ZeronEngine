// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/WindowModule.h"
#include "Events/EventDispatcher.h"
#include "Events/EventTypes/EventTypes.h"
#include "Window/WindowGLFW.h"

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
		CreateNewWindow<WindowGLFW>(WindowProps(m_EventDispatcher));
	}

	void WindowModule::Destroy()
	{

	}

	void WindowModule::Update()
	{

		
		// Update all windows
		for(const auto& window : m_WindowContainer )
		{
			// Update every window that is managed by this module
			window.second->Update();
		}
		
		// Delete windows that are scheduled to be removed
		while(m_WindowRemoveContainer.empty() == false)
		{
			// Dispatch event for main window closure
			if(m_WindowRemoveContainer.front() == 0){

				m_WindowContainer.clear();
				m_EventDispatcher->Dispatch<Events::Window::MainClose>(Events::Window::MainClose());

				// Clear remove queue
				std::queue<int> empty;
				std::swap(m_WindowRemoveContainer, empty);
				return;
				
			}
			
			m_WindowContainer.erase(m_WindowRemoveContainer.front());
			m_WindowRemoveContainer.pop();
		}
	}
	
	bool WindowModule::HasWindow(const WindowHandle& handle) const
	{
		return handle.m_WindowCreator == this && m_WindowContainer.count(handle.GetHandleID()) > 0;
	}

	Window* WindowModule::GetMainWindow() const
	{
		return nullptr;
	}

	Window* WindowModule::GetWindow(int windowID) const
	{
		if(m_WindowContainer.count(windowID) > 0)
		{
			return m_WindowContainer.at(windowID).get();
		}

		return nullptr;
	}

	Window* WindowModule::GetWindow(const WindowHandle& handle) const
	{
		if (HasWindow(handle))
		{
			return m_WindowContainer.at(handle.GetHandleID()).get();
		}

		return nullptr;
	}
	
	bool WindowModule::RemoveWindow(const WindowHandle& handle)
	{
		if (HasWindow(handle))
		{
			m_WindowRemoveContainer.push(handle.GetHandleID());
			return true;
		}

		return false;
	}

	bool WindowModule::RemoveAll()
	{
		

		
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
