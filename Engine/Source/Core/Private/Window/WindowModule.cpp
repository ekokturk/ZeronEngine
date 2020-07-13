// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/WindowModule.h"


#include "Application.h"
#include "Events/EventDispatcher.h"
#include "Events/EventTypes/EventTypes.h"
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
		//const_cast<WindowContext*>(a.WindowContext)->SetAttention();
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
			// Remove window
			ZERON_LOG_INFO("Window '{}' is closed...", m_WindowRemoveContainer.front().WindowContext->GetName())
			m_WindowContextContainer.erase(m_WindowRemoveContainer.front());
			m_WindowRemoveContainer.pop();

			// Send all windows closed event
			if (m_WindowContextContainer.empty())
			{
				m_EventDispatcher->Dispatch<Events::Window::CloseAll>(Events::Window::CloseAll());
			}
		}
	}


	WindowContext* WindowModule::GetWindow(const WindowContextHandle& Handle) const
	{
		if (m_WindowContextContainer.count(Handle) > 0)
		{
			return m_WindowContextContainer.at(Handle).get();
		}

		return nullptr;
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

		// WINDOW CLOSED
		Dispatcher.Register<Events::Window::Close>(this, [=](const Events::Window::Close& e)
		{
			e.Consume();
			RemoveWindow(e.ContextHandle); // Schedule to remove
		});
	}
}
