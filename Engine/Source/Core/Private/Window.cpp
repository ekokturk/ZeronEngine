// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "../Public/Window.h"
#include "Events/Event.h"

namespace ZeronEngine
{
	Window::Window()
	{
	}

	Window::~Window()
	{
	}

	void Window::Init()
	{
	}

	void Window::Update()
	{
	}

	void Window::Destroy()
	{
		Event e;
		e.m_Type = EventType::WindowClosed;
		m_EventCallback(e);
	}

	void Window::SetEventCallback(std::function<void(Event&)>& callback)
	{
		m_EventCallback = callback;
	}
}
