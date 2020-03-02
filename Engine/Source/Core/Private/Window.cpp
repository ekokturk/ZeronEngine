// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "../Public/Window.h"
#include "Events/Event.h"

namespace ZeronEngine
{
	Window::Window()
		:m_Height(0),
		m_Width(0),
		m_Name("Zeron Engine")
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
		EventType::WindowClosed data;
		m_EventCallback(Event(data));
	}

	void Window::SetEventCallback(std::function<void(Event&)>& callback)
	{
		m_EventCallback = callback;
	}
}
