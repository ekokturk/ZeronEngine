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
		// TODO Remove this after implementation
		const auto key = std::make_pair(InputActionBinding(MouseCode(MouseCode::LeftButton), ModifierKeys()), InputState::Released);
		auto callback = []() {ZERON_LOG_INFO("Worked"); };
		m_ActionCallbacks[key].push_back(std::make_pair(this, callback));
	}

	InputModule::~InputModule()
	{
		
	}


	void InputModule::SetInputMapping(std::shared_ptr<InputMapping> newInputMapping)
	{
		m_CurrentInputMapping = std::move(newInputMapping);
	}

	bool InputModule::BindActionInput(void* owner, const std::string& inputName, InputState inputState, const InputActionCallback& callback)
	{
		InputActionBinding actionBinding;
		if(m_CurrentInputMapping->GetActionBinding(inputName,actionBinding) && owner)
		{
			const auto key = std::make_pair<InputActionBinding, InputState>(std::move(actionBinding), std::move(inputState));
			m_ActionCallbacks[key].push_back(callback);
		}
		
		return false;
	}

	bool InputModule::BindPollInput(const std::string& inputName, const InputPollCallback& callback)
	{
		InputPollBinding pollBinding;
		if (m_CurrentInputMapping->GetPollBinding(inputName, pollBinding))
		{
			m_PollCallbacks[pollBinding].push_back(callback);
		}
		
		return false;
	}

	bool InputModule::UnbindActionInput(const std::string& inputName)
	{
		return false;
	}

	bool InputModule::UnbindPollInput(const std::string& inputName)
	{
		return false;
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

		});


		Dispatcher.Register<Events::Mouse::Move>(this, [=](const Events::Mouse::Move& e)
		{
			//if (m_Mouse)
			//{
				ZERON_LOG("X{} Y{}", e.CurrentPos.X - e.PreviousPos.X, e.CurrentPos.Y - e.PreviousPos.Y)
			//}
		});


		Dispatcher.Register<Events::Mouse::Release>(this, [=](const Events::Mouse::Release& e)
		{
			const auto actionPair = std::make_pair<InputActionBinding, InputState>
				(InputActionBinding(InputType(e.Button), e.Modifiers), InputState::Released);

			if (m_ActionCallbacks.count(actionPair))
			{
				m_ActionCallbacks[actionPair][0].second();
			}
			
			ZERON_LOG("{} Released", e.Button.ToString())
		});

		Dispatcher.Register<Events::Key::Press>(this, [=](const Events::Key::Press& e)
		{
			// If we are listening for key, set key value for polled input

			// If there is a press input action trigger it


			
			ZERON_LOG("{} Pressed", e.Key.ToString())
			if(e.Key == KeyCode::Enter && e.Modifiers.HasKey(ModifierKeys::Alt))
			{
				if(m_FocusedWindow.Get())
				{
					m_FocusedWindow.Get()->SetFullScreen(!m_FocusedWindow.Get()->IsFullScreen());
				}
			}
		});

		Dispatcher.Register<Events::Key::Release>(this, [](const Events::Key::Release& e)
		{
			// If we are listening for key, unset key value for polled input

			// If there is a press input action trigger it

			
		});

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
