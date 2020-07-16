// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Input/InputModule.h"

#include "Logger.h"
#include "Events/EventTypes/EventTypes.h"
#include "Events/EventDispatcher.h"
#include "Input/Mouse.h"

namespace ZeronEngine
{
	InputModule::InputModule()
	{
		m_Mouse = std::make_unique<Mouse>();

	}

	InputModule::~InputModule()
	{
		
	}


	void InputModule::SetInputMapping(std::shared_ptr<InputMapping> newInputMapping)
	{
		m_CurrentInputMapping = std::move(newInputMapping);
	}

	bool InputModule::BindInput(MouseCode mouseCode, MouseInputState inputState, const std::function<void()>& callback)
	{
		if(m_Mouse)
		{
			m_Mouse->BindMouseInput(mouseCode, inputState, callback);
		}

		ZERON_LOG_WARNING("Binding mouse input '{}' failed. Mouse device is not initialized!")
		
		return false;
	}

	bool InputModule::BindInput(const std::string& mouseEvent, MouseInputState inputState,
		const std::function<void()>& callback)
	{
		// Get mouse code from input mapping
		const MouseCode mouseCode = m_CurrentInputMapping->GetMouseCode(mouseEvent);
		if(!mouseCode)
		{
			ZERON_LOG_WARNING("Binding mouse input '{}' failed. Input name does not exist!", mouseEvent)
			return false;
		}
		
		return BindInput(mouseCode, inputState, callback);
	}


	void InputModule::Init()
	{
	}

	void InputModule::Update()
	{
		
	}

	void InputModule::Destroy()
	{
	}
	
	void InputModule::RegisterEvents(const EventDispatcher& Dispatcher)
	{
		Dispatcher.Register<Events::Window::Focus>(this, [=](const Events::Window::Focus& e)
		{
			m_FocusedWindow = e.ContextHandle;
		});
		Dispatcher.Register<Events::Window::Unfocus>(this, [=](const Events::Window::Unfocus& e)
		{
			m_FocusedWindow = WindowHandle();
		});

		Dispatcher.Register<Events::Mouse::Enter>(this, [=](const Events::Mouse::Enter& e)
		{
			m_HoveredWindow = e.ContextHandle;
		});
		Dispatcher.Register<Events::Mouse::Exit>(this, [=](const Events::Mouse::Exit& e)
		{
			m_HoveredWindow = WindowHandle();
		});
		

		Dispatcher.Register<Events::Mouse::Press>(this, [=](const Events::Mouse::Press& e)
		{
			if(m_Mouse)
			{
				//m_Mouse->Broadcast(e.Button);
				//if(e.Button == )
				//{
				//	e.ContextHandle.Get()->SetFullScreen(true);
				//}
			}
		});



		Dispatcher.Register<Events::Mouse::Release>(this, [](const Events::Mouse::Release& e)
		{
			ZERON_LOG("{} Released", e.Button.ToString())
		});

		Dispatcher.Register<Events::Key::Press>(this, [=](const Events::Key::Press& e)
		{
			ZERON_LOG("{} Pressed", e.Key.ToString())
			if(e.Key == KeyCode::Enter && e.Modifiers.HasKey(ModifierKeys::Alt))
			{
				if(m_FocusedWindow.Get())
				{
					m_FocusedWindow.Get()->SetFullScreen(!m_FocusedWindow.Get()->IsFullScreen());
				}
			}
		});

		//Dispatcher.Register<Events::Key::Release>(this, [](const Events::Key::Release& e)
		//{
		//	ZERON_LOG("{} Released", e.Key.ToString())
		//});
		//Dispatcher.Register<Events::Key::Repeat>(this, [](const Events::Key::Repeat& e)
		//{
		//	ZERON_LOG("{} Repeat", e.Key.ToString())
		//});
		//Dispatcher.Register<Events::Key::Character>(this, [](const Events::Key::Character& e)
		//{
		//	ZERON_LOG("\u{} Pressed", "\u"+e.UTF8)
		//});

	}

	Vector2 InputModule::GetMousePosition() const
	{
		if(m_FocusedWindow.IsValid())
		{
			return m_FocusedWindow.Get()->GetMousePosition();
		}

		return Vector2();
	}
}
